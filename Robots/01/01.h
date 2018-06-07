#ifdef BASEROBOT_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#include "../../Battle_Robots/Options.h"

void StartPosSearch(Stepinfo* si, double &limit);
void PosDefend(Stepinfo* si, double& limit);

extern "C" {

	API void DoStep(Stepinfo*);
}

