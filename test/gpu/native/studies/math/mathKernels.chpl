use Math;
import Time.stopwatch;
use Random;

config const size:uint(32) = 100_000_000;
config const iterations = 1;
config const printTime = true;
config const correctness = false;
config param function = "tanh";


var D = {0..size};
var CpuA : [D] real(32);

extern cu_nvcc_main(printTime: int, correctness: int): void;
extern cu_clang_main(printTime: int, correctness: int): void;

proc main() {
  fillRandom(CpuA);

  on here.gpus[0]{
    var A = CpuA;
    const D = A.domain;

    var s : stopwatch;
    s.start();
    kernel(function, D, A);
    s.stop();
    if printTime then
      writeln("Chapel Time: ", s.elapsed()* 1e3, " ms");
    if correctness then
      writeln("Chapel Sum: ", + reduce A);
  }

  cu_nvcc_main(printTime, correctness);
  cu_clang_main(printTime, correctness);


}

inline kernel(param function: string, const D, ref A) {
  @assertOnGpu
  @gpu.blockSize(256)
  foreach i in D {
    select function {
      when "tanh" do A[i] = tanh(i:real(32));
      otherwise compilerError("Unknown function: " + function);
    }
  }
}
