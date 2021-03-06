// 14.cpp: определяет процедуры инициализации для библиотеки DLL.
//
#include "stdafx.h"
#include "14.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define MAP_2TO1(R,C,W) (R*W+C)
#define MAP_1TO2(K,R,C,W) {R=K/W;C=K-R*W;}

using namespace std;

void charge(Stepinfo* si, double limit);
void attack(Stepinfo* si, double limit);
void repair(Stepinfo* si, double limit);
void _getGridIJ(double x, double y, int &r, int &c);
void getDestPoint(double start_x, double start_y, double &end_x, double &end_y, bool isAttack);
enum class State
{
	CHARGE,
	ATTACK,
	REPAIR
};
enum class State state = State::CHARGE;
bool *charge_flags = 0;
bool isCharging = FALSE;
map<CString, bool> attack_flags;
double grid_side = -1;
int grid_r = -1;
int grid_c = -1;
int **grid = 0;
int *dist = 0;
int *from = 0;
bool *visit = 0;
int W = -1;
int H = -1;

void DoStep(Stepinfo* si)
{
#pragma region INIT_CHARGE_FLAGS
	if (!charge_flags) charge_flags = new bool[si->chargePoints.size()];
#pragma endregion

#pragma region INIT_GRID
	if (!grid)
	{
		grid_side = si->settings.R_max * 4;
		H = si->settings.H;
		W = si->settings.W;
		grid_r = H / grid_side;
		if (H / grid_side != 0.0) grid_r++;
		grid_c = W / grid_side;
		if (W / grid_side != 0.0) grid_c++;
		grid = new int*[grid_r];
		for (int i = 0; i < grid_r; i++) grid[i] = new int[grid_c];
		dist = new int[grid_r * grid_c];
		from = new int[grid_r * grid_c];
		visit = new bool[grid_r * grid_c];
	}

	for (int i = 0; i < grid_r; i++)
	{
		for (int j = 0; j < grid_c; j++)
		{
			grid[i][j] = 0;
			dist[MAP_2TO1(i, j, grid_c)] = 0;
			from[MAP_2TO1(i, j, grid_c)] = -1;
			visit[MAP_2TO1(i, j, grid_c)] = FALSE;
		}
	}

	for (auto it = si->robots.begin(); it != si->robots.end(); it++)
	{
		if (it->second.name == si->robot.name ||
			it->second.fname == "01.dll" ||
			it->second.fname == "16.dll" ||
			it->second.fname == "07.dll" ||
			it->second.fname == "14.dll") continue;
		int i, j;
		_getGridIJ(it->second.X, it->second.Y, i, j);
		if (i<grid_r && j<grid_c)
			grid[i][j]++;
	}
#pragma endregion

#pragma region MAKE_CHARGE_FLAGS_MAP
	isCharging = FALSE;
	int radius = si->settings.R_max * 2;
	for (int i = 0; i < si->chargePoints.size(); i++)
	{
		charge_flags[i] = false;
		for (auto it = si->robots.begin(); it != si->robots.end(); it++)
		{
			attack_flags[it->second.fname] = FALSE;
			int dist = sqrt(pow(si->chargePoints[i].X - it->second.X, 2) +
				pow(si->chargePoints[i].Y - it->second.Y, 2));
			if (it->second.fname == si->robot.fname && dist < radius)
			{
				isCharging = TRUE;
			}
			if (it->second.fname == si->robot.fname ||
				it->second.fname == "01.dll" ||
				it->second.fname == "16.dll" ||
				it->second.fname == "07.dll" ||
				it->second.fname == "14.dll") continue;
			if (dist < radius)
			{
				charge_flags[i] = true;
			}
			if (dist < 2 * radius)
			{
				attack_flags[it->second.fname] = TRUE;
			}
		}
	}
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

	for (int i = 0; i < si->chargePoints.size(); i++)
	{
		if (si->robot.E > si->settings.E_max * 75 / 100 &&
			charge_flags[i] && !isCharging) continue;
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
		if (steps == 0)steps = 1;

		double dx, dy;
		double destX = si->chargePoints[minI].X;
		double destY = si->chargePoints[minI].Y;
		getDestPoint(si->robot.X, si->robot.Y, destX, destY, FALSE);
		dx = ((int)destX - si->robot.X) / steps;
		dy = ((int)destY - si->robot.Y) / steps;

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
			it->second.E > 0 && it->second.E < minE &&
			!attack_flags[it->second.fname] &&
			it->second.fname != "01.dll" &&
			it->second.fname != "16.dll" &&
			it->second.fname != "07.dll" &&
			it->second.fname != "14.dll" 
			)
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
				double destX = si->robots[minName].X;
				double destY = si->robots[minName].Y;
				getDestPoint(si->robot.X, si->robot.Y, destX, destY, TRUE);
				dx = ((int)destX - si->robot.X) / steps;
				dy = ((int)destY - si->robot.Y) / steps;
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
		double destX = si->remontPoints[minI].X;
		double destY = si->remontPoints[minI].Y;
		getDestPoint(si->robot.X, si->robot.Y, destX, destY, FALSE);
		dx = ((int)destX - si->robot.X) / steps;
		dy = ((int)destY - si->robot.Y) / steps;

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

void _getGridIJ(double x, double y, int &r, int &c)
{
	r = y != H ? y / grid_side : grid_r - 1;
	c = x != W ? x / grid_side : grid_c - 1;
}

void getDestPoint(double start_x, double start_y, double &end_x, double &end_y, bool isAttack)
{
	return;
	int r, c;
	_getGridIJ(start_x, start_y, r, c);
	int finishr, finishc;
	_getGridIJ(end_x, end_y, finishr, finishc);
	if (r == finishr && c == finishc) return;

	for (int i = 0; i < grid_r; i++)
	{
		for (int j = 0; j < grid_c; j++)
		{
			if (i != r && j != c)
			{
				dist[MAP_2TO1(i, j, grid_c)] = INT_MAX;
			}
		}
	}

	int visit_count = 1;
	while (visit_count != grid_r * grid_c)
	{
		int minDist = INT_MAX;
		int minI = -1;
		for (int i = 0; i < grid_r * grid_c; i++)
		{
			if (!visit[i] && dist[i] < minDist)
			{
				minDist = dist[i];
				minI = i;
			}
		}
		visit[minI] = TRUE;
		visit_count++;
		MAP_1TO2(minI, r, c, grid_c)
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if (i != 0 && j != 0)
					{
						int nr = r + i;
						int nc = c + j;
						if (nr >= 0 && nr < grid_r &&
							nc >= 0 && nc < grid_c)
						{
							int map = MAP_2TO1(nr, nc, grid_c);
							if (!visit[map])
							{
								int cost = grid[nr][nc] * 10000 + 1;;
								if (isAttack)
								{
									if (finishr == nr && finishc == nc &&
										grid[nr][nc] == 1) cost = 1;
								}
								if (dist[map] > dist[minI] + cost)
								{
									dist[map] = dist[minI] + cost;
									from[map] = minI;
								}
							}
						}
					}
				}
			}
	}

	int endr, endc;
	_getGridIJ(end_x, end_y, endr, endc);
	int endmap = MAP_2TO1(endr, endc, grid_c);
	if (from[endmap] != -1)
	{
		while (from[from[endmap]] != -1)
		{
			endmap = from[endmap];
		}
	}
	MAP_1TO2(endmap, endr, endc, grid_c)
		end_x = ((2 * endc + 1) * grid_side) / 2;
	end_y = ((2 * endr + 1) * grid_side) / 2;
}