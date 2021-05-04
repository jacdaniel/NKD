# CMakeList.txt : projet CMake pour Geotime, incluez la source et définissez
# la logique spécifique au projet ici.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FIND_PACKAGE(CUDA REQUIRED)
find_package(OpenMP REQUIRED)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

message(CMAKE_EXE_LINKER_FLAGS)

INCLUDE_DIRECTORIES( ${CUDA_INCLUDE_DIRS})
link_directories(${CUDA_INCLUDE_DIRS}/../lib/x64)

enable_language(CUDA) 

# add_definitions("--std=c++11")
set (CMAKE_CXX_STANDARD 11)


# Ajoutez une source à l'exécutable de ce projet.
add_executable (
Geotime 
"./main.cpp" "./main.h"
"./src/Utils/utils.cpp" "./src/Utils/utils.h"
"./src/Utils/ThreadUtil.cpp" "./src/Utils/ThreadUtil.h"
"./src/solver/poissonSolver.cpp" "./src/solver/poissonSolver.h"
"./src/solver/poissonFourier.cpp" "./src/solver/poissonFourier.h"
"./src/solver/poissonFourierCPU.cpp" "./src/solver/poissonFourierCPU.h"
"./src/solver/laplacian.cu" "./src/solver/laplacian.h"
"./src/SignalProcessing/OpNablaCPU.cpp" "./src/SignalProcessing/OpNablaCPU.h"  "run/runPoissonSolver.cpp" "run/runPoissonSolver.h"

"./run/runPoissonSolver.cpp" "./run/runPoissonSolver.h"
 "run/utilDisplay.cpp" "run/utilDisplay.h" 
 "src/SurfaceReconstruction/surfaceReconstruction.cpp" "src/SurfaceReconstruction/surfaceReconstruction.h" 
 "src/SurfaceReconstruction/surfaceReconstructionCPU.cpp" "src/SurfaceReconstruction/surfaceReconstructionCPU.h" 
 "src/SurfaceReconstruction/normalManager.h" "src/SurfaceReconstruction/normalManager.cpp" 
 "src/SurfaceReconstruction/normalManagerCPU.cpp" "src/SurfaceReconstruction/normalManagerCPU.h" 
 "run/runSurfaceReconstruction.cpp" "run/runSurfaceReconstruction.h" 
 "src/SurfaceStacking/surfaceStacking.cpp" "src/SurfaceStacking/surfaceStacking.h" 
 "src/solver/conjugateGradient.cpp" "src/solver/conjugateGradient.h" 
 "src/solver/conjugateGradientCallBack.h" "src/solver/conjugateGradientCallBack.cpp" 
 "src/SignalProcessing/OpDotCPU.cpp" "src/SignalProcessing/OpDotCPU.h" 
 "src/solver/conjugateGradientCPU.cpp" "src/solver/conjugateGradientCPU.h" 
 "src/SignalProcessing/OpMemCpyCPU.h" "src/SignalProcessing/OpMemCpyCPU.cpp" 
 "src/Arithmetic/arithmetic_YPlusEqualsAlphaTimesX_CPU.cpp" "src/Arithmetic/arithmetic_YPlusEqualsAlphaTimesX_CPU.h" 
 "src/Arithmetic/arithmetic.h" 
 "src/Arithmetic/arithmetic_YMinusEqualsAlphaTimesX_CPU.cpp" 
 "src/Arithmetic/arithmetic_YMinusEqualsAlphaTimesX_CPU.h" 
 "run/runConjugateGradient.cpp" "run/runConjugateGradient.h" 
 "src/Arithmetic/arithmetic_YEqualsXPlusAlphaTimesY_CPU.h" 
 "src/Arithmetic/arithmetic_YEqualsXPlusAlphaTimesY_CPU.cpp" 
 "src/XData/xData.h" "src/XData/xData.cpp" "run/runXDataTest.h" "run/runXDataTest.cpp" "src/MemoryProcessing/OpMalloc_CPU.h" "src/MemoryProcessing/OpMalloc_CPU.cpp")

target_include_directories(Geotime 
PRIVATE "./"
PRIVATE "./src/Utils"
PRIVATE "./src/solver"
PRIVATE "./src/SignalProcessing"
PRIVATE "./src/SurfaceReconstruction"
PRIVATE "./run"
PRIVATE "./src/SurfaceStacking"
PRIVATE "./src/Arithmetic"
PRIVATE "./src/XData"
PRIVATE "./src/MemoryProcessing"
PRIVATE "D:/JACK2/DEV/C/NKDEEP/Geotime/lib")

message("-- Linking SDL ")

set(OpenMP_CXX_FLAGS "-Xclang -fopenmp")
target_link_directories(Geotime PRIVATE "./lib") 
target_link_libraries(Geotime PRIVATE libfftw3-3)
target_link_libraries(Geotime PRIVATE libfftw3f-3)
# target_link_libraries(Geotime PUBLIC libopm)
# target_link_libraries(Geotime PUBLIC libomp5)

# TODO: Ajoutez des tests et installez des cibles si nécessaire.
