use Time;
use Random;
use CTypes;

config const printTime = false;
config const printCorrectness = false;

config const initArray = true;
config param manualConstFold = false;

param A: real(32) = 1.0;
param B: real(32) = 20.0;
param C: real(32) = 25.6;
param D: real(32) = 24.0;


config param N = 100_000_000;
config param iters = 100;
config const seed = 0;
config type ARRAY_TYPE = int(64);

var nums: [0..<N] ARRAY_TYPE;

proc kernel() {
  var sum: real(32) = 0.0;
  foreach i in 0..<N with (ref sum) {
    param one: real(32) = 1.0;
    if !manualConstFold {
      const x: real(32) =
          if nums[i] % 3 == 0
          then (if nums[i] % 5 == 0 then A else B)
          else (if nums[i] % 4 == 0 then C else D);

      const rx: real(32) = one / x;
      sum += rx;
    } 
    else {
      const rx: real(32) =
          if nums[i] % 3 == 0
          then (if nums[i] % 5 == 0 then one / A else one / B)
          else (if nums[i] % 4 == 0 then one / C else one / D);

      sum += rx;
    }
  }
  return sum;
}


proc main() {

  if initArray {
    var rs = if seed == 0
              then new randomStream(nums.eltType)
              else new randomStream(nums.eltType, seed);
    // init backwards to preserve cache
    for i in 0..<N by -1 {
      nums[i] = rs.next();
    }
  }

  var dest = new c_array(real(32), iters);

  var s = new stopwatch();
  s.start();
  for i in 0..<iters {
    dest[i] = kernel();
  }
  s.stop();
  if printTime then writeln("chpl Time: ", s.elapsed());


  var sum: real(32);
  for i in 0..<iters {
    sum += dest[i];
  }
  if printCorrectness then writef("%6.2dr\n", sum);

}
