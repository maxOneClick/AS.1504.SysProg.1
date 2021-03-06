// 01.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "stdafx.h"
#include <ctime>
#include <cstdlib>
#include <math.h>

#include "01.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

void DoStep(Stepinfo* si)
{
	double lim = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;

	int buffer = 0;
	int dist;

	int sX = si->settings.W / 2;
	int sY = si->settings.H / 2;

	int smin = 1000;
	bool flag = false;
	for (int i = 0; i < si->chargePoints.size(); i++)
	{
		dist = sqrt(pow(si->chargePoints[i].X - si->robot.X, 2) + pow(si->chargePoints[i].Y - si->robot.Y, 2));

		for (auto curBot = si->robots.begin(); curBot != si->robots.end(); ++curBot)
		{
			if ((curBot->second.fname != "01.dll" && curBot->second.fname != "14.dll" && 
				curBot->second.fname != "16.dll" && curBot->second.fname != "07.dll") 
				&& curBot->second.E > 0)
			{
				int dEnemy = sqrt(pow(si->chargePoints[i].X - curBot->second.X, 2) + pow(si->chargePoints[i].Y - curBot->second.Y, 2));

				if (dEnemy <= 3 * lim)
					flag = true;
			}
		}
		if (smin > dist && !flag)
		{
			smin = dist;
			buffer = i;
		}
		flag = false;
	}

	dist = sqrt(pow(si->chargePoints[buffer].X - si->robot.X, 2) + pow(si->chargePoints[buffer].Y - si->robot.Y, 2));
	
	if (dist < lim)
	{
		si->curActions.X = si->chargePoints[buffer].X;
		si->curActions.Y = si->chargePoints[buffer].Y;
	}
	else
	{
		int step = dist / lim + 1;

		double dx, dy;
		dx = (si->chargePoints[buffer].X - si->robot.X) / step;
		dy = (si->chargePoints[buffer].Y - si->robot.Y) / step;

		si->curActions.X = si->robot.X + dx;
		si->curActions.Y = si->robot.Y + dy;
	}

	int min = 10000;
	CString buf;

	for (auto curBot = si->robots.begin(); curBot != si->robots.end(); ++curBot)
	{
		dist = sqrt(pow(curBot->second.X - si->robot.X, 2) + pow(curBot->second.Y - si->robot.Y, 2));

		if (curBot->second.E > 0 && (curBot->second.fname != "01.dll" && curBot->second.fname != "14.dll" &&
			curBot->second.fname != "16.dll" && curBot->second.fname != "07.dll"))
		{
			if (min > dist)
			{
				min = dist;
				buf = curBot->first;
			}
		}
	}

	int r = si->settings.R_max*si->robot.V / si->settings.L_max * si->robot.E / si->settings.E_max;

	if (min <= r && si->robot.E > 0)
	{
		si->curActions.targetName = buf;
	}
}
