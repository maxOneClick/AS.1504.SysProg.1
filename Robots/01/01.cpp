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

vector <Robot> friends;

void DoStep(Stepinfo* si)
{
	for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
	{
		if (it->second.fname == "01.dll" && it->second.fname == "14.dll" && it->second.fname == "16.dll")
			friends.push_back(it->second);
	}

	double lim = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;


	StartPosSearch(si, lim);
	PosDefend(si, lim);
}


void StartPosSearch(Stepinfo* si, double& limit)
{
	int buffer = 0;
	int dist;

	int sX, sY;

	if (si->stepNum < 250)
	{
		sX = 0.5 * si->settings.W;
		sY = 0.5 * si->settings.H;
	}
	else
	{
		if (si->stepNum < 501 && si->stepNum >= 250)
		{
			sX = 0.6 * si->settings.W;
			sY = 0.2 * si->settings.H;
		}
		else
		{
			sX = si->settings.W / 2;
			sY = si->settings.H / 2;
		}
	}

	int smin = 10000;
	bool flag = false;
	for (int i = 0; i < si->chargePoints.size(); i++)
	{
		dist = sqrt(pow(si->chargePoints[i].X - sX, 2) + pow(si->chargePoints[i].Y - sY, 2));

		for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
		{
			if ((it->second.fname != "14.dll") && it->second.E > 0)
			{
				int dEnemy = sqrt(pow(si->chargePoints[i].X - it->second.X, 2) + pow(si->chargePoints[i].Y - it->second.Y, 2));

				if (dEnemy <= 3 * limit)
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
	if (dist < limit)
	{
		si->curActions.X = si->chargePoints[buffer].X;
		si->curActions.Y = si->chargePoints[buffer].Y;
	}
	else
	{
		int step = dist / limit + 1;

		double dx, dy;
		dx = (si->chargePoints[buffer].X - si->robot.X) / step;
		dy = (si->chargePoints[buffer].Y - si->robot.Y) / step;

		si->curActions.X = si->robot.X + dx;
		si->curActions.Y = si->robot.Y + dy;
	}
}


void PosDefend(Stepinfo* si, double& limit)
{
	int dist;
	int min = 10000;
	CString buf;
	for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
	{
		dist = sqrt(pow(it->second.X - si->robot.X, 2) + pow(it->second.Y - si->robot.Y, 2));
		if (it->second.E > 0 && it->second.fname != "13_Robot.dll")
		{
			if (min > dist)
			{
				min = dist;
				buf = it->first;
			}
		}
	}
	int r = si->settings.R_max*si->robot.V / si->settings.L_max * si->robot.E / si->settings.E_max;
	if (min <= r && si->robot.E > 0)
	{
		si->curActions.targetName = buf;
	}
}
