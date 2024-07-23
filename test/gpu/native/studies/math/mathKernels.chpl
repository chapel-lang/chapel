use Math;
import Time.stopwatch;
use Random;

config const size:uint(32) = 100_000_000;
config const iterations:int(32) = 1;
config const printTime = true;
config const correctness = false;
config param function = "tanhf";

extern ("cu_nvcc_" + function + "_main")
proc cu_nvcc_main(printTime: int, correctness: int, iterations: int): void;
extern ("cu_clang_" + function + "_main")
proc cu_clang_main(printTime: int, correctness: int, iterations: int): void;


var D = {0..size};
var CpuA : [D] real(32);

proc main() {
  fillRandom(CpuA);

  on here.gpus[0]{
    var A = CpuA;
    const D = A.domain;

    var s : stopwatch;
    s.start();
    kernel(function, D, A, iterations);
    s.stop();
    if printTime then
      writeln("Chapel Time: ", s.elapsed()* 1e3, " ms");
    if correctness then
      writeln("Chapel Sum: ", + reduce A);
  }

  cu_nvcc_main(printTime, correctness, iterations);
  cu_clang_main(printTime, correctness, iterations);


}

inline proc kernel(param function: string, const D, ref A, iters) {
  @assertOnGpu
  @gpu.blockSize(256)
  foreach i in D {
    var s: real(32) = 0;
    for i in 0..<iters do
      s += callFunc(function, i);
    A[i] = s;
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
