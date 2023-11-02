/* PF_COMP_UNIT_TYPE determines the behavior of the accelerated compilation unit
	 ------------------------------------------------------------
   CUDA
	 ------------------------------------------------------------
	 1:     NVCC compiler, Unified Memory allocation, Parallel loops on GPUs
	 2:     NVCC compiler, Unified Memory allocation, Sequential loops on host
	 Other: NVCC compiler, Standard heap allocation, Sequential loops on host

	 ------------------------------------------------------------
   OpenMP
	 ------------------------------------------------------------
     1:     CXX compiler, Unified Memory allocation, Parallel loops on CPU
	 2:     CXX compiler, Unified Memory allocation, Sequential loops on CPU
	 Other: CXX compiler, Standard heap allocation, Sequential loops on CPU
*/

// This definition is now controlled by CMake (./CMakeLists.txt)
// #define PF_COMP_UNIT_TYPE 0

/* extern "C" is required for the C source files when compiled with CPP compiler */
extern "C"{
  #include "grgeometry.c"
}
