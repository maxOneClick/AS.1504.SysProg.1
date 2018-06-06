// 15.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "stdafx.h"
#include "15.h"
#include <ctime>
#include <cstdlib>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

void DoStep(Stepinfo* si)
{
	double max_path = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;

	int path;
	int saved = 0;
	bool vr;

	if (si->robot.Y >= 0.75*si->settings.W)
		vr = true;
	else
		vr = false;

	switch (vr)
	{
	case true:
	{
		int r_y = si->robot.Y;

		for (int i = 0; i < si->chargePoints.size(); i++)
		{
			if (si->chargePoints[i].Y > r_y)
			{
				r_y = si->chargePoints[i].Y;
				saved = i;
			}
		}

		path = sqrt(pow(si->chargePoints[saved].X - si->robot.X, 2) + pow(si->chargePoints[saved].Y - si->robot.Y, 2));

		break;
	}

	case false:
	{
		int dist;
		path = 99999;

		for (int i = 0; i < si->chargePoints.size(); i++)
		{
			if (si->chargePoints[i].X >= 0.25 * si->settings.W && si->chargePoints[i].Y >= 0.25 * si->settings.H)
			{
				dist = sqrt(pow(si->chargePoints[i].X - si->robot.X, 2) + pow(si->chargePoints[i].Y - si->robot.Y, 2));

				if (path > dist)
				{
					path = dist;
					saved = i;
				}
			}

		}

		break;
	}
	default:
		break;
	}


	if (path < max_path)
	{
		si->curActions.X = si->chargePoints[saved].X;
		si->curActions.Y = si->chargePoints[saved].Y;
	}
	else
	{
		int steps;
		steps = path / max_path + 1;

		double dx, dy;
		dx = (si->chargePoints[saved].X - si->robot.X) / steps;
		dy = (si->chargePoints[saved].Y - si->robot.Y) / steps;

		si->curActions.X = si->robot.X + dx;
		si->curActions.Y = si->robot.Y + dy;
	}

	path = 0;
	int min = 99999;
	CString saved_2;

	for (auto r = si->robots.begin(); r != si->robots.end(); ++r)
	{
		if (r->second.fname != "15.dll" && r->second.fname != "09.dll" && r->second.fname != "10.dll")
		{
			path = sqrt(pow(r->second.X - si->robot.X, 2) + pow(r->second.Y - si->robot.Y, 2));
			if (path != 0 && r->second.E != 0)
			{
				if (min > path)
				{
				min = path;
				saved_2 = r->first;
				}
			}
		}
	}

	int attack_radius = si->settings.R_max*si->robot.V / si->settings.L_max * si->robot.E / si->settings.E_max;
	double energy_now = si->settings.E_max / 3;
	if (min <= attack_radius && si->robot.E > energy_now)
	{
		si->curActions.targetName = saved_2;
	}
}
