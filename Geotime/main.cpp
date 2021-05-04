// Geotime.cpp : définit le point d'entrée de l'application.
//

#include <runPoissonSolver.h>
#include <runSurfaceReconstruction.h>
#include <runConjugateGradient.h>
#include <runXDataTest.h>
#include "main.h"

using namespace std;

int main()
{
	// runPoissonSolver(0, nullptr);
	// runSurfaceReconstruction();
	runConjugateGradient(0, nullptr);

	// runXDataTest(0, nullptr);
	cout << "Hello CMake 30" << endl;
	return 0;
}
