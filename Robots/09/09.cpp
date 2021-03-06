// 09.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "stdafx.h"
#include "09.h"
#include <ctime>
#include <cstdlib>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

void DoStep(Stepinfo* si)
{
	vector <Robot> Friends;
	for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
	{
		if (it->second.fname == "09.dll" || it->second.fname == "10.dll" || it->second.fname == "15.dll")
			Friends.push_back(it->second);
	}

	/*si->robot.A = 0;
	si->robot.P = 50;
	si->robot.V = 50;*/
	
	int destination = 0;
	int min = 9999999;
	double limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;
	double distance;

		if (si->robot.E <= 0.6*si->settings.E_max) {
			for (int i = 0; i < si->chargePoints.size(); i++)
			{
				distance = sqrt(pow(si->chargePoints[i].X - si->robot.X, 2) + pow(si->chargePoints[i].Y - si->robot.Y, 2));
				if (min > distance)
				{
					min = distance;
					destination = i;
				}
			}

			if (min <= limit)
			{
				si->curActions.X = si->chargePoints[destination].X;
				si->curActions.Y = si->chargePoints[destination].Y;
			}
			else
			{
				int steps;
				steps = min / limit + 1;
				double dx, dy;
				dx = (si->chargePoints[destination].X - si->robot.X) / steps;
				dy = (si->chargePoints[destination].Y - si->robot.Y) / steps;

				si->curActions.X = si->robot.X + dx;
				si->curActions.Y = si->robot.Y + dy;
			}
		}
		else {
			if (si->robot.L <= 0.5*si->settings.L_max) {
				for (int i = 0; i < si->remontPoints.size(); i++)
				{
					distance = sqrt(pow(si->remontPoints[i].X - si->robot.X, 2) + pow(si->remontPoints[i].Y - si->robot.Y, 2));
					if (min > distance)
					{
						min = distance;
						destination = i;
					}
				}

				if (min <= limit)
				{
					si->curActions.X = si->remontPoints[destination].X;
					si->curActions.Y = si->remontPoints[destination].Y;
				}
				else
				{
					int steps;
					steps = min / limit + 1;
					double dx, dy;
					dx = (si->remontPoints[destination].X - si->robot.X) / steps;
					dy = (si->remontPoints[destination].Y - si->robot.Y) / steps;

					si->curActions.X = si->robot.X + dx;
					si->curActions.Y = si->robot.Y + dy;
				}
			}
			else {
				destination = 0;
				int min = 99999;
				CString enemy;

				for (auto i = si->robots.begin(); i != si->robots.end(); ++i)
				{
					destination = sqrt(pow(i->second.X - si->robot.X, 2) + pow(i->second.Y - si->robot.Y, 2));
					if (destination != 0 && i->second.E != 0)
					{
						if (min > destination)
						{
							min = destination;
							enemy = i->first;
						}
					}
				}

				si->robot.A = 0;
				si->robot.P = 0;
				si->robot.V = 100;

				int attack_radius = si->settings.R_max*si->robot.V / si->settings.L_max * si->robot.E / si->settings.E_max;
				if (min <= attack_radius)
				{
					si->robot.A = 60;
					si->robot.P = 30;
					si->robot.V = 10;
					si->curActions.targetName = enemy;
				}
				else {
				}
			}
		}
}


