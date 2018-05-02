#include "stdafx.h"
#include <ctime>
#include <cstdlib>
#include <math.h>

#include "BaseRobot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

void DoStep(Stepinfo* si)
{
	srand(time(NULL)*GetCurrentThreadId());

	double limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;


	if (limit < 1)
	{
		si->curActions.dV = 10;
		si->curActions.dP = -5;
		si->curActions.dA = -5;

		limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;
	}

	int variant = rand();
	variant = variant % 4;

	Point point;

	switch (variant)
	{
		////	Движение робота в случайном направлении		////
	case 1:
	{
		si->curActions.X = si->robot.X + (rand() % 5 - rand() % 5);
		si->curActions.Y = si->robot.Y + (rand() % 5 - rand() % 5);

		break;
	}
	////	Поиск и движение к ближайшей станции зарядки	////
	case 2:
	{
		int distance;
		int min = 9999;
		int buffer = 0;

		for (int i = 0; i < si->chargePoints.size(); i++)
		{
			distance = sqrt(pow(si->chargePoints[i].X - si->robot.X, 2) + pow(si->chargePoints[i].Y - si->robot.Y, 2));

			if (min > distance)
			{
				min = distance;
				buffer = i;
			}
		}

		if (min <= limit)
		{
			si->curActions.X = si->chargePoints[buffer].X;
			si->curActions.Y = si->chargePoints[buffer].Y;
		}
		else
		{
			int steps;
			steps = min / limit + 1;

			if (steps != 0)
			{
				double dx, dy;
				dx = (si->chargePoints[buffer].X - si->robot.X) / steps;
				dy = (si->chargePoints[buffer].Y - si->robot.Y) / steps;

				si->curActions.X = si->robot.X + dx;
				si->curActions.Y = si->robot.Y + dy;
			}

		}

		break;
	}
	////	Поиск и атака ближайшего вражеского робота		////
	case 3:
	{
		int distance;
		int min = 9999;
		CString buffer;

		for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
		{
			distance = sqrt(pow(it->second.X - si->robot.X, 2) + pow(it->second.Y - si->robot.Y, 2));

			if (distance != 0)
			{
				if (min > distance && si->robots[it->first].E > 0 )
				{
					min = distance;
					buffer = it->first;
				}
			}
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

			if (steps != 0) 
			{
				dx = (si->robots[buffer].X - si->robot.X) / steps;
				dy = (si->robots[buffer].Y - si->robot.Y) / steps;
			}

			si->curActions.X = si->robot.X + dx;
			si->curActions.Y = si->robot.Y + dy;
		}

		break;
	}
	////	Поиск и движение к ближайшей станции ремонта	////
	case 4:
	{
		int distance;
		int min = 9999;
		int buffer = 0;

		for (int i = 0; i != si->remontPoints.size(); i++)
		{
			distance = sqrt(pow(si->remontPoints[i].X - si->robot.X, 2) + pow(si->remontPoints[i].Y - si->robot.Y, 2));

			if (min > distance)
			{
				min = distance;
				buffer = i;
			}
		}

		if (min < limit)
		{
			si->curActions.X = si->remontPoints[buffer].X;
			si->curActions.Y = si->remontPoints[buffer].Y;
		}
		else
		{
			int steps;
			steps = min / limit + 1;

			if (steps != 0)
			{
				double dx, dy;
				dx = (si->remontPoints[buffer].X - si->robot.X) / steps;
				dy = (si->remontPoints[buffer].Y - si->robot.Y) / steps;

				si->curActions.X = si->robot.X + dx;
				si->curActions.Y = si->robot.Y + dy;
			}
		}

		break;
	}
	////	Поиск трупа и дальнейшее его мародерство	////
	////	НУЖНЫ ДОРАБОТКИ
	case 5:
	{
		int distance;
		int min = 9999;
		CString buffer;

		for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
		{
			distance = sqrt(pow(it->second.X - si->robot.X, 2) + pow(it->second.Y - si->robot.Y, 2));

			if (distance != 0)
			{
				if (min > distance)
				{
					min = distance;
					buffer = it->first;
				}
			}
		}

		int steps;
		steps = min / limit + 1;

		double dx = 0, dy = 0;

		if (steps != 0)
		{
			dx = (si->robots[buffer].X - si->robot.X) / steps;
			dy = (si->robots[buffer].Y - si->robot.Y) / steps;
		}
		si->curActions.X = si->robot.X + dx;
		si->curActions.Y = si->robot.Y + dy;

		break;
	}
	default:break;
	}

}
