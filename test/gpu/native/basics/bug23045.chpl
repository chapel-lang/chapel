// this code is from https://github.com/chapel-lang/chapel/issues/23045 verbatim
// the bug reported will be resolved via
// https://github.com/chapel-lang/chapel/pull/23048

use GPU;
use CTypes;

extern {
  static __device__ __host__ void printInt(int x) {
    printf("%d\n", x);
  }
}

pragma "codegen for GPU"
extern proc printInt(x : c_int);

proc testValue(){
  var exp = 1234;

  on here.gpus[0]{
    @assertOnGpu
    foreach i in 0 .. 0 {
      printInt(exp:c_int); // prints garbage value
    }
  }
  exp = exp + 1; // Without this line there's no error and the printInt function prints the right value
}

testValue();
