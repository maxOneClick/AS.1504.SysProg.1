// 05.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "stdafx.h"
#include "05.h"
#include <ctime>
#include <cstdlib>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

void DoStep(Stepinfo* si)
{
	double limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;

	if (si->chargePoints.size() != 0)
	{
		double distance;
		double min = 9999;
		int imin = 0;


		for (int i = 0; i < si->chargePoints.size(); i++)
		{
			distance = sqrt(pow(si->chargePoints[i].X - si->robot.X, 2) + pow(si->chargePoints[i].Y - si->robot.Y, 2));

			if (min > distance)
			{
				min = distance;
				imin = i;
			}
		}



		if (min != 0)
		{
			//ищем ближайшую зарядку и идем к ней

			if (min <= limit)
			{
				si->curActions.X = si->chargePoints[imin].X;
				si->curActions.Y = si->chargePoints[imin].Y;
			}
			else
			{
				int steps;
				steps = min / limit + 1;

				double dx, dy;
				dx = (si->chargePoints[imin].X - si->robot.X) / steps;
				dy = (si->chargePoints[imin].Y - si->robot.Y) / steps;

				si->curActions.X = si->robot.X + dx;
				si->curActions.Y = si->robot.Y + dy;
			}
			return;
		}
	}


	//если мы в точке заряда или их нету
	double distance;
	double min = 9999;
	CString buffer;
	auto robot = si->robots.begin()->second;

	for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
	{
		if (it->second.fname != "5.dll")
		{
			distance = sqrt(pow(it->second.X - si->robot.X, 2) + pow(it->second.Y - si->robot.Y, 2));
			if (min > distance && si->robots[it->first].E > 0)
			{
				min = distance;
				buffer = it->first;
				robot = it->second;
			}
		}
	}

	if (min < limit && min != 0 && robot.fname != "")
	{
		//враг на расстоянии передвижения к нему за шаг
		double dx, dy;
		dx = (si->robot.X - robot.X);
		dy = (si->robot.Y - robot.Y);

		si->curActions.X = si->robot.X + dx;
		si->curActions.Y = si->robot.Y + dy;
		return;
	}

	int radius = si->settings.R_max*si->robot.V / si->settings.L_max * si->robot.E / si->settings.E_max;

	if (min <= radius && si->robot.E > 0)
	{
		si->curActions.targetName = buffer;

	}
	else
	{
		int steps;
		steps = min / limit + 1;

		double dx = 0, dy = 0;

		if (steps != 0) {
			dx = (si->robots[buffer].X - si->robot.X) / steps;
			dy = (si->robots[buffer].Y - si->robot.Y) / steps;
		}
		si->curActions.X = si->robot.X + dx;
		si->curActions.Y = si->robot.Y + dy;
	}
}


