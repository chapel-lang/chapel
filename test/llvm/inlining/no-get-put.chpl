//
// This test is a simpler version of the kinds of patterns that can cause
// performance issues in `test/studies/lcals/`. When compiled with
// `--no-local`, GETS, PUTS, and memmove wreak havoc on performance. This test
// is a regression test to make sure these are always inlined properly.
//

use Time;
config const samples=15000;
config const N = 171;
config type T = real;

const s_num_1D_Real_arrays = 16;
const aligned_chunksize = 44256;

class LoopData {
  var RealArray_1D: [0..#s_num_1D_Real_arrays][0..#aligned_chunksize] real;
}
var loop_data = new LoopData();

proc initData(ref ra: [] real, id: int) {
  const factor: real = if id % 2 != 0 then 0.1 else 0.2;
  for (r,j) in zip(ra, 0..) {
    r = factor*(j + 1.1)/(j + 1.12345);
  }
}
proc loopInit() {
  for i in 1..5 {
    initData(loop_data.RealArray_1D[i-1], i);
  }
}


var s = new stopwatch();

proc kernel(samples, len) {
  ref out1 = loop_data.RealArray_1D[0];
  ref out2 = loop_data.RealArray_1D[1];
  ref out3 = loop_data.RealArray_1D[2];
  ref in1  = loop_data.RealArray_1D[3];
  ref in2  = loop_data.RealArray_1D[4];

  s.start();
  for 0..#samples do
    for i in 0..#len {
      out1[i] = in1[i] * in2[i];
      out2[i] = in1[i] + in2[i];
      out3[i] = in1[i] - in2[i];
    }
  s.stop();
}

proc main() {
  loopInit();
  kernel(samples, N);
  writeln("Elapsed time: ", s.elapsed(), " seconds");
}
