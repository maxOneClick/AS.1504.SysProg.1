#pragma once
#include "Options.h"

struct Result {
	CString name;
	double energy;
	double kills;
	int steps;
	int score;
};

class Core
{
private:
	void initMap();
	void loadRobots();
public:
	int steps;
	int roundNum = 1;
	int count = 0;
	Settings settings;
	map<CString, Robot> robots;
	vector<Robot> winners;
	map<CString, Robot> futureRobots;
	vector<Point> remontPoints;
	vector<Point> chargePoints;
	vector<Result> results;
	int robotsCount;
	int deadC;
	CString robotsFilePath = "123.txt";
	void initCore();
	void startBattle();
	void loadSettings(int);
	int aliveCount;
	void battleStep();
	double getRand();
	Core();
	~Core();
};

