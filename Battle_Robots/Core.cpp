#include "stdafx.h"
#include "Core.h"
#include <fstream>
#include <string>

using namespace std;

Core::Core()
{
	loadSettings(1);
	steps = 0;
	robotsCount = 0;
	aliveCount = 0;
	settings.W = 200;
	settings.H = 200;
	settings.N = 1000;
	settings.T = 100;
	settings.E_max = 1000;
	settings.L_max = 100;
	settings.V_max = 10;
	settings.R_max = 5;
	settings.dL = 10;
	settings.dE_S = 1;
	settings.N_E = 10;
	settings.dE_V = 2;
	settings.dE_A = 10;
	settings.dE = 100;
	settings.N_L = 10;
	settings.K = 100;
	settings.RND_min = 0.4;
	settings.RND_max = 0.8;
	settings.dE_P = 5;
}

// считываем из settings все парамеры
void Core::loadSettings(int round) {
	CString fname;
	fname.Format("Settings\\Round %d.txt", round);
	ifstream f(fname);
	{
		string line;

		getline(f, line);
		robotsFilePath = line.substr(line.find(' ') + 3).c_str();

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.W = atoi(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.H = atoi(line.c_str());


		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.N = atoi(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.T = atoi(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.E_max = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.L_max = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.V_max = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.R_max = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.dL = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.dE_S = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.dE_V = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.dE_A = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.dE_P = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.dE = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.N_E = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.N_L = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.RND_min = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.RND_max = atof(line.c_str());

		getline(f, line);
		line = line.substr(line.find(' ') + 3);
		settings.K = atoi(line.c_str());
	}
}

//задаем положения для станций

void Core::initMap() {
	for (int i = 0; i < settings.N_E; i++) {
		Point p;
		p.X = rand() % settings.W;
		p.Y = rand() % settings.H;
		chargePoints.push_back(p);
	}
	for (int i = 0; i < settings.N_L; i++) {
		Point p;
		p.X = rand() % settings.W;
		p.Y = rand() % settings.H;
		remontPoints.push_back(p);
	}
}

//загрузка роботов

void Core::loadRobots() {
	ifstream fin(robotsFilePath);
	robots.clear();
	while (1) {
		if (fin.eof()) break;
		string fname;
		fin >> fname;
		string name;
		fin >> name;

		int k;
		fin >> k;

		for (int j = 0; j < k; j++) {
			HMODULE hLib = LoadLibrary(fname.c_str());
			Robot robot;
			robot.DoStep = (PF)GetProcAddress(hLib, "DoStep");
			robot.E = settings.E_max;
			robot.L = settings.L_max;
			robot.A = robot.L / 3;
			robot.P = robot.L / 3;
			robot.V = robot.L / 3;
			robot.kills = 0;
			CString s;
			s.Format("(%d)", j);
			robot.name = !j ? name.c_str() : name.c_str() + s; // если выжил
			robot.fname = fname.c_str();
			robot.count = k;
			robot.X = rand() % settings.W;
			robot.Y = rand() % settings.H;
			robots[robot.name] = robot;
		}
	}

	for (size_t i = 0; i < winners.size(); i++) {
		Robot robot = winners[i];
		robot.E = settings.E_max;
		robot.L = settings.L_max;
		robot.A = robot.L / 3;
		robot.P = robot.L / 3;
		robot.V = robot.L / 3;
		robot.kills = 0;
		robot.X = rand() % settings.W;
		robot.Y = rand() % settings.H;

		if (robot.name.Right(1) == ")") {
			robot.name = robot.name.Left(robot.name.Find("(", 1));
		}

		CString name;
		int k = 0;
		do {
			k++;
			name.Format(robot.name + "(%d)", k);
		} while (robots.find(name) != robots.end());

		robot.name = name;

		robots[name] = robot;
	}



	for (int i = 0; i < 5; i++) {
		Robot robot;
		HMODULE hLib = LoadLibrary("BaseRobot.dll");
		robot.DoStep = (PF)GetProcAddress(hLib, "DoStep");
		robot.E = settings.E_max;
		robot.L = settings.L_max;
		robot.A = robot.L / 3;
		robot.P = robot.L / 3;
		robot.V = robot.L / 3;
		robot.kills = 0;
		CString name;
		int k = 0;
		do {
			k++;
			name.Format("BaseRobot(%d)", k);
		} while (robots.find(name) != robots.end());
		robot.name = name;
		robot.X = rand() % settings.W;
		robot.Y = rand() % settings.H;
		robots[name] = robot;
	}

	futureRobots = robots;
	count++;
}

void Core::initCore() {
	steps = 0;
	robots.clear();
	futureRobots.clear();
	remontPoints.clear();
	chargePoints.clear();
	aliveCount = 0;
	results.clear();
	loadRobots();
	initMap();
}

struct Msg {
	Stepinfo * si;
	PF DoStep;
};


UINT roboThread(LPVOID lpParam) {
	Msg * msg = (Msg*)lpParam;
	msg->DoStep(msg->si);
	return 0;
}

void Core::startBattle() {

}

double Core::getRand() {
	int max = settings.RND_max * 100.;
	int min = settings.RND_min * 100.;
	return (min + rand() % (max - min)) / 100.;
}

void Core::battleStep() {
	aliveCount = 0;
	futureRobots = robots;
	deadC = 0;
	steps++;
	map<CString, vector<CString>> maybeDead;
	if (steps == 1000) roundNum++;
	for (auto it = robots.begin(); it != robots.end(); ++it) {
		Robot robot = it->second;
		Robot futureRobot = futureRobots[it->first];
		if (robot.E <= 0) { deadC++; continue; }
		aliveCount++;
		Stepinfo * si = new Stepinfo;
		si->settings = settings;
		si->chargePoints = chargePoints;
		si->remontPoints = remontPoints;
		si->robots = robots;
		si->stepNum = steps;
		si->robot = it->second;
		si->curActions.dP = 0;
		si->curActions.dA = 0;
		si->curActions.dV = 0;
		Msg * msg = new Msg;
		msg->si = si;
		msg->DoStep = robot.DoStep;

		CWinThread * hThread = AfxBeginThread(roboThread, msg);
		DWORD dwCode = WaitForSingleObject(hThread->m_hThread, settings.T);
		if (dwCode == WAIT_FAILED) {
			TerminateThread(hThread->m_hThread, 0);
		}
		else {
			for (size_t i = 0; i < chargePoints.size(); i++) {
				if (si->curActions.X == chargePoints[i].X && si->curActions.Y == chargePoints[i].Y) {
					futureRobot.E += settings.dE;
					futureRobot.E = futureRobot.E > settings.E_max ? settings.E_max : futureRobot.E;
					break;
				}
			}

			for (size_t i = 0; i < remontPoints.size(); i++) {
				if (si->curActions.X == remontPoints[i].X && si->curActions.Y == remontPoints[i].Y) {
					futureRobot.L += settings.dL;
					futureRobot.L = futureRobot.L > settings.L_max ? settings.L_max : futureRobot.L;
					double dL = futureRobot.L - robot.L;
					futureRobot.A += dL / 3;
					futureRobot.V += dL / 3;
					futureRobot.P += dL / 3;
					break;
				}
			}

			for (auto it1 = robots.begin(); it1 != robots.end(); ++it1) {
				if (it1->second.E <= 0) {
					if (robot.X == it1->second.X && robot.Y == it1->second.Y) {
						double dL = 0;
						if (it1->second.L > 0) {
							dL = settings.dL > it1->second.L ? it1->second.L : settings.dL;
							robot.A += dL / 3;
							robot.V += dL / 3;
							robot.L += dL / 3;
							it1->second.L -= dL;
						}
					}
				}
			}
			if ((robot.X == si->curActions.X && robot.Y == si->curActions.Y) || si->curActions.X <= 0 || si->curActions.Y <= 0 || si->curActions.X > settings.W || si->curActions.Y > settings.H) {
				futureRobot.E -= settings.dE_S;
			}
			else {
				int sMax = settings.V_max * robot.V / settings.L_max * robot.E / settings.E_max;
				int sCur = sqrt(pow(robot.X - si->curActions.X, 2) + pow(robot.Y - si->curActions.Y, 2));
				if (sCur <= sMax) {
					futureRobot.X = si->curActions.X;
					futureRobot.Y = si->curActions.Y;
					futureRobot.E -= settings.dE_P;
				}
				else {
					futureRobot.E -= settings.dE_S;
				}
			}
			if (si->curActions.targetName != "") {
				if (robots[si->curActions.targetName].E > 0) {
					futureRobot.E -= settings.dE_A;
					futureRobots[si->curActions.targetName].E -= settings.dE_P;
					double Ar = getRand()*robot.A;
					double Pr = (1 - getRand())*robots[si->curActions.targetName].P;
					double As = 3 * Ar * robot.E / settings.E_max;   
					double Ps = Pr * robots[si->curActions.targetName].E / settings.E_max;  

					if (As > Ps) {
						int dP = As - Ps;
						futureRobots[si->curActions.targetName].P -= dP;
						futureRobots[si->curActions.targetName].L -= dP;

						if (futureRobots[si->curActions.targetName].P <= 0) {
							dP = abs(futureRobots[si->curActions.targetName].P);
							futureRobots[si->curActions.targetName].A -= dP / 2;
							futureRobots[si->curActions.targetName].V -= dP / 2;
							futureRobots[si->curActions.targetName].P = 0;
							int dE = abs(Ps - As) * settings.E_max / settings.L_max;
							futureRobots[si->curActions.targetName].E -= dE;

							if (futureRobots[si->curActions.targetName].E <= 0) {
								maybeDead[si->curActions.targetName].push_back(robot.name);
							}
						}
					}
					else {
						int dA = Ps - As;
						futureRobot.A -= dA;
						futureRobot.L -= dA;
						if (futureRobot.A <= 0) {
							dA = abs(futureRobot.A);
							futureRobot.A = 0;
							futureRobot.P = dA / 2;
							futureRobot.V = dA / 2;
							int dE = abs(Ps - As) * settings.E_max / settings.L_max;
							futureRobot.E -= dE;
							if (futureRobot.E <= 0) {
								futureRobots[si->curActions.targetName].kills++;
							}
						}
					}

				}
			}
			if (si->curActions.dA != 0 || si->curActions.dV != 0 || si->curActions.dP != 0) {

				double newA = robot.A + si->curActions.dA;
				double newP = robot.P + si->curActions.dP;
				double newV = robot.V + si->curActions.dV;
				if (newA >= 0 && newP >= 0 && newV >= 0 && round(newA + newP + newV) == robot.L) {
					futureRobot.A += si->curActions.dA;
					futureRobot.V += si->curActions.dV;
					futureRobot.P += si->curActions.dP;
				}
			}
			si->curActions.step = steps;

			futureRobot.prevAction = si->curActions;
			futureRobots[robot.name] = futureRobot;
		}
		delete si;
	}

	for (auto it = maybeDead.begin(); it != maybeDead.end(); ++it) {
		if (futureRobots[it->first].E <= 0) {
			for (size_t i = 0; i < it->second.size(); i++) {
				futureRobots[it->second[i]].kills += 1. / it->second.size();
			}

		}
	}
	maybeDead.clear();
	robots = futureRobots;
}


Core::~Core()
{
}
