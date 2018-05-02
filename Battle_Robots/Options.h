#pragma once

#include <iostream>
#include <map>
#include <vector>

using namespace std;

struct Stepinfo;

typedef void(*PF)(Stepinfo*);

struct Action {
	int X, Y;
	CString targetName;
	double dA, dP, dV;
	int step;
};

struct Robot {
	CString name;
	CString fname;
	double E, L, A, P, V;
	double kills;
	PF DoStep;
	int X, Y;
	Action prevAction;
	int count;
};

struct Settings {
	int W, H, N, T, K;
	double E_max, L_max, V_max, R_max, dL, dE_S, dE_V, dE_A, dE_P, dE, N_E, N_L;
	double RND_min, RND_max;
};

struct Point {
	int X, Y;
};


struct Stepinfo {
	int stepNum;
	map<CString, Robot> robots;
	Robot robot;
	vector<Point> remontPoints;
	vector<Point> chargePoints;
	Action curActions;
	Settings settings;
};

