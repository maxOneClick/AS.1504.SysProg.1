#ifdef BASEROBOT_EXPORTS
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#include "../../Battle_Robots/Options.h"

extern "C" {
	API void DoStep(Stepinfo*);
}
