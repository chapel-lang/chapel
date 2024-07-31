#ifndef MATH_KERNELS_H_
#define MATH_KERNELS_H_

#define MAKE_FUNC(func) \
  void cu_nvcc_##func##_main(int printTime, int correctness, int iterations); \
  void cu_clang_##func##_main(int printTime, int correctness, int iterations);

MAKE_FUNC(tanhf)
MAKE_FUNC(sqrtf)
MAKE_FUNC(fabsf)

#endif
