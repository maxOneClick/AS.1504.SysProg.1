// 07.cpp: определяет процедуры инициализации для библиотеки DLL.
//

#include "stdafx.h"
#include "07.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
using namespace std;

void charge(Stepinfo* si, double limit);
void attack(Stepinfo* si, double limit);
void repair(Stepinfo* si, double limit);
enum class State
{
	CHARGE,
	ATTACK,
	REPAIR
};
enum class State state = State::CHARGE;
bool *charge_flags = 0;

void DoStep(Stepinfo* si)
{
#pragma region INIT_FLAGS
	if (!charge_flags) charge_flags = new bool[si->chargePoints.size()];
#pragma endregion

#pragma region LIMIT
	double limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;
	if (limit < 1)
	{
		si->curActions.dV = 10;
		si->curActions.dP = -5;
		si->curActions.dA = -5;

		limit = si->settings.V_max*si->robot.V / si->settings.L_max*si->robot.E / si->settings.E_max;
	}
#pragma endregion

#pragma region SWITCH_STATE
	switch (state)
	{
	case State::CHARGE:
		charge(si, limit);
		break;
	case State::ATTACK:
		attack(si, limit);
		break;
	case State::REPAIR:
		repair(si, limit);
		break;
	default:
		state = State::CHARGE;
		break;
	}
#pragma endregion

#pragma region CHANGE_APV

#pragma endregion
}

void charge(Stepinfo* si, double limit)
{
#pragma region MOVE_AND_CHARGE
	int minDist = sqrt(pow(si->chargePoints[0].X - si->robot.X, 2) +
		pow(si->chargePoints[0].Y - si->robot.Y, 2));
	int minI = 0;

#pragma region MAKE_FLAGS
	int radius = si->settings.R_max * 2;
	for (int i = 0; i < si->chargePoints.size(); i++)
	{
		charge_flags[i] = false;
		for (auto it = si->robots.begin(); it != si->robots.end(); it++)
		{
			if (it->second.name == si->robot.name) continue;
			int dist = sqrt(pow(si->chargePoints[i].X - it->second.X, 2) +
				pow(si->chargePoints[i].Y - it->second.Y, 2));
			if (dist < radius) charge_flags[i] = true;
		}
	}
#pragma endregion

	for (int i = 0; i < si->chargePoints.size(); i++)
	{
		if (si->robot.E > si->settings.E_max * 75 / 100 &&
			charge_flags[i]) continue;
		int currDist = sqrt(pow(si->chargePoints[i].X - si->robot.X, 2) +
			pow(si->chargePoints[i].Y - si->robot.Y, 2));
		if (minDist > currDist)
		{
			minDist = currDist;
			minI = i;
		}
	}

	if (minDist < limit)
	{
		si->curActions.X = si->chargePoints[minI].X;
		si->curActions.Y = si->chargePoints[minI].Y;
	}
	else
	{
		int steps;
		steps = minDist / limit + 1;

		double dx, dy;
		dx = (si->chargePoints[minI].X - si->robot.X) / steps;
		dy = (si->chargePoints[minI].Y - si->robot.Y) / steps;

		si->curActions.X = si->robot.X + dx;
		si->curActions.Y = si->robot.Y + dy;
	}
#pragma endregion

#pragma region CALC_E_ENEMIES
	CString minName = "";
	int minE = si->settings.E_max;
	if (!si->robots.empty())
	{
		for (auto it = si->robots.begin(); it != si->robots.end(); it++)
		{
			if (it->second.name != si->robot.name &&
				it->second.E > 0 && it->second.E < minE)
			{
				minE = it->second.E;
				minName = it->first;
			}
		}
	}
#pragma endregion

#pragma region CHANGE_STATE
	if (si->robot.E > si->settings.E_max * 9 / 10 &&

		si->robot.L == si->settings.L_max &&

		minName.Compare("") != 0 &&
		si->robots[minName].E * 8 / 5 < si->robot.E &&
		si->stepNum < si->settings.N * 9 / 10)
	{
		state = State::ATTACK;
	}
	if (si->robot.E > si->settings.E_max * 7 / 10 &&
		si->robot.L < si->settings.L_max * 9 / 10 &&
		si->stepNum < si->settings.N * 9 / 10)
	{
		state = State::REPAIR;
	}
#pragma endregion
}

void attack(Stepinfo* si, double limit)
{
#pragma region MOVE_AND_ATTACK
	CString minName = "";
	int minE = si->settings.E_max;
	for (auto it = si->robots.begin(); it != si->robots.end(); it++)
	{
		if (it->second.name != si->robot.name &&
			it->second.E > 0 && it->second.E < minE)
			if (
				it->second.fname != "01.dll" &&
				it->second.fname != "14.dll" &&
				it->second.fname != "16.dll" &&
				it->second.fname != "07.dll")
			{
				minE = it->second.E;
				minName = it->first;
			}
	}

	if (minName.Compare("") != 0 && si->robots[minName].E < si->robot.E)
	{
		int dist = sqrt(pow(si->robots[minName].X - si->robot.X, 2) +
			pow(si->robots[minName].Y - si->robot.Y, 2));
		int radius = si->settings.R_max*si->robot.V /
			si->settings.L_max * si->robot.E / si->settings.E_max;
		if (dist <= radius && si->robot.E > 0)
		{
			si->curActions.targetName = minName;
		}
		else
		{
			int steps;
			steps = dist / limit + 1;
			double dx = 0, dy = 0;
			if (steps != 0)
			{
				dx = (si->robots[minName].X - si->robot.X) / steps;
				dy = (si->robots[minName].Y - si->robot.Y) / steps;
			}
			si->curActions.X = si->robot.X + dx;
			si->curActions.Y = si->robot.Y + dy;
		}
	}
#pragma endregion

#pragma region CHANGE_STATE
	if (si->robot.E < si->settings.E_max * 75 / 100 ||
		minName.Compare("") == 0 ||
		si->robots[minName].E > si->robot.E)
	{
		state = State::CHARGE;
	}
	if (si->robot.E > si->settings.E_max * 8 / 10 &&
		si->robot.L < si->settings.L_max * 7 / 10)

	{
		state = State::REPAIR;
	}
#pragma endregion
}

void repair(Stepinfo* si, double limit)
{
#pragma region MOVE_AND_REPAIR
	int minDist = sqrt(pow(si->remontPoints[0].X - si->robot.X, 2) +
		pow(si->remontPoints[0].Y - si->robot.Y, 2));
	int minI = 0;

	for (int i = 0; i < si->remontPoints.size(); i++)
	{
		int currDist = sqrt(pow(si->remontPoints[i].X - si->robot.X, 2) +
			pow(si->remontPoints[i].Y - si->robot.Y, 2));
		if (minDist > currDist)
		{
			minDist = currDist;
			minI = i;
		}
	}

	if (minDist < limit)
	{
		si->curActions.X = si->remontPoints[minI].X;
		si->curActions.Y = si->remontPoints[minI].Y;
	}
	else
	{
		int steps;
		steps = minDist / limit + 1;

		double dx, dy;
		dx = (si->remontPoints[minI].X - si->robot.X) / steps;
		dy = (si->remontPoints[minI].Y - si->robot.Y) / steps;

		si->curActions.X = si->robot.X + dx;
		si->curActions.Y = si->robot.Y + dy;
	}
#pragma endregion

#pragma region CHANGE_STATE
	if (si->robot.E < si->settings.E_max * 65 / 100 ||

		si->robot.L >= si->settings.L_max * 9 / 10)
	{
		state = State::CHARGE;
	}
#pragma endregion
}



