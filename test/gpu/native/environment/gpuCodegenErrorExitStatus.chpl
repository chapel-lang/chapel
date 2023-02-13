// This test causes an error in GPU codegen because the gpu function 'foo' is
// not found. The GPU codegen process exits with a code of 1, but the main
// process was exiting with a code of 0. This was causing problems with
// start_test because the compiler returned success but didn't generate
// an executable. The compiler is now correctly returning != 0 when GPU
// codegen fails, so this test now passes.

use GPU;
pragma "codegen for GPU"
extern proc foo() : void;

on here.gpus[0] {
  foreach i in 0..10 {
    assertOnGpu();
    foo();
  }
}
