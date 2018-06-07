// 16.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "stdafx.h"
#include "16.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

void DoStep(Stepinfo* si)
{
	int StartPointX = 0.5*si->settings.W;
	int	StartPointY = 0.5*si->settings.H;
	int BasePoint = 0;
	int BasePoint2 = 0;


	bool alert = false;



	int min = 9999999;
	int  distance = 0;
	if (si->chargePoints.size() != 0)
	{
		for (int i = 0; i < si->chargePoints.size(); i++)
		{
			distance = sqrt(pow(si->chargePoints[i].X - StartPointX, 2) + pow(si->chargePoints[i].Y - StartPointY, 2));

			for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
			{

				int dEnemy = sqrt(pow(si->chargePoints[i].X - it->second.X, 2) + pow(si->chargePoints[i].Y - it->second.Y, 2));

				if (dEnemy <= 2 * min)
					alert = true;


			}



			if (min > distance && !alert)
			{
				min = distance;
				BasePoint = i;
			}
			alert = false;

			distance = sqrt(pow(si->chargePoints[BasePoint].X - si->robot.X, 2) + pow(si->chargePoints[BasePoint].Y - si->robot.Y, 2));
		}
		min = 99999999;
		if (si->stepNum >= si->settings.N / 1.8)
		{
			for (int i = 0; i < si->chargePoints.size(); i++)
			{
				int  distance = sqrt(pow(si->chargePoints[i].X - si->chargePoints[BasePoint].X, 2) + pow(si->chargePoints[i].Y - si->chargePoints[BasePoint].Y, 2));
				if (min > distance && distance > 0)
				{
					min = distance;
					BasePoint2 = i;
				}
			}
			BasePoint = BasePoint2;
		}
		int distanceToBase = sqrt(pow(si->chargePoints[BasePoint].X - si->robot.X, 2) + pow(si->chargePoints[BasePoint].Y - si->robot.Y, 2));
		double BaseRadius = si->settings.R_max;
		if (distanceToBase > BaseRadius)
		{
			double limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;
			if (distanceToBase < limit)
			{
				si->curActions.X = si->chargePoints[BasePoint].X;
				si->curActions.Y = si->chargePoints[BasePoint].Y;
			}
			else
			{
				int steps;
				steps = distanceToBase / limit + 1;
				if (steps <= 0)
					steps = 1;
				double dx, dy;
				dx = (si->chargePoints[BasePoint].X - si->robot.X) / steps;
				dy = (si->chargePoints[BasePoint].Y - si->robot.Y) / steps;

				si->curActions.X = si->robot.X + dx;
				si->curActions.Y = si->robot.Y + dy;
			}
		}
		else
		{
			double limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;
			CString enemy;
			enemy = "";
			for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
			{
				int distanceEnemyToBase = sqrt(pow(si->chargePoints[BasePoint].X - it->second.X, 2) + pow(si->chargePoints[BasePoint].Y - it->second.Y, 2));
				bool en = true;

			
				if (it->second.fname == "16.dll" || it->second.fname == "14.dll" || it->second.fname == "01.dll" &&
					it->second.fname == "07.dll" || si->robot.name == it->first)
				{
					en = false;
					break;
				}

				int distanceToEnemy = sqrt(pow(si->robot.X - it->second.X, 2) + pow(si->robot.Y - it->second.Y, 2));
				int radius = si->settings.R_max*si->robot.V / si->settings.L_max * si->robot.E / si->settings.E_max;
				if (distanceEnemyToBase <= BaseRadius && en && it->second.E > 0 && distanceToEnemy <= radius && si->robot.E > 0)
				{
					enemy = it->first;
					break;
				}
			}
			if (enemy != "" && si->robot.E > 0.75*si->settings.E_max)
			{
				si->curActions.targetName = enemy;
			}
			else
			{
				double limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;
				if (distanceToBase < limit)
				{
					si->curActions.X = si->chargePoints[BasePoint].X;
					si->curActions.Y = si->chargePoints[BasePoint].Y;
				}
				else
				{
					int steps;
					steps = distanceToBase / limit + 1;
					if (steps <= 0)
						steps = 1;
					double dx, dy;
					dx = (si->chargePoints[BasePoint].X - si->robot.X) / steps;
					dy = (si->chargePoints[BasePoint].Y - si->robot.Y) / steps;

					si->curActions.X = si->robot.X + dx;
					si->curActions.Y = si->robot.Y + dy;
				}
			}
		}
	}

	else
	{
		CString enemy;
		enemy = "";
		for (auto it = si->robots.begin(); it != si->robots.end(); ++it)
		{
			bool en = true;

			
			if (it->second.fname == "16.dll" || it->second.fname == "14.dll" || it->second.fname == "01.dll" &&
				it->second.fname == "07.dll" || si->robot.name == it->first)
			{
				en = false;
				break;
			}

			int distanceToEnemy = sqrt(pow(si->robot.X - it->second.X, 2) + pow(si->robot.Y - it->second.Y, 2));
			int radius = si->settings.R_max*si->robot.V / si->settings.L_max * si->robot.E / si->settings.E_max;
			if (en && it->second.E > 0 && distanceToEnemy <= radius && si->robot.E > 0)
			{
				enemy = it->first;
				break;
			}
		}
		if (enemy != "" && si->robot.E > 0.75*si->settings.E_max)
		{
			si->curActions.targetName = enemy;
		}
	}
}