// This test checks that we can call extern functions from the GPU
// This specifically uses `printf` which can be bothersome

extern { static __device__ __host__ void externHello() {  printf("hello\n"); }; }

pragma "codegen for GPU"
extern proc externHello();

on here.gpus[0] {
  foreach i in 1..1 {
    externHello();
  }
}
