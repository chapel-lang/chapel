use Math;
import Time.stopwatch;
use Random;

config const size:uint(32) = 100_000_000;
config const printTime = true;
config const correctness = false;
config param function = "tanhf";

extern ("cu_nvcc_" + function + "_main")
proc cu_nvcc_main(printTime: int, correctness: int): void;
extern ("cu_clang_" + function + "_main")
proc cu_clang_main(printTime: int, correctness: int): void;


var D = {0..size};
var CpuA : [D] real(32);

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

inline proc kernel(param function: string, const D, ref A) {
  @assertOnGpu
  @gpu.blockSize(256)
  foreach i in D {
    A[i] = callFunc(function, i);
  }
}
inline proc callFunc(param function: string, v) {
  select function {
    when "tanhf" do return tanh(v:real(32));
    when "sqrtf" do return sqrt(v:real(32));
    when "fabsf" do return abs(v:real(32));
    otherwise compilerError("Unknown function: " + function);
  }
}
