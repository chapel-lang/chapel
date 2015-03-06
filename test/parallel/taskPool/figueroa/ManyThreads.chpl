// This test is intended to ensure that a warning message is emitted when
// the specified value of CHPL_RT_NUM_THREADS_PER_LOCALE is higher than the
// system supports (256 for gasnet, for example).

use Time;

config const numThreads = 3000;
var total: sync int = 0;
var seed = getCurrentTime() : uint(32);

proc foo (x) {

  proc RandomNumber (y) {
    const multiplier: int(64) = 16807,
          modulus: int(64) = 2147483647;
    // The following calculation must be done in at least 46-bit arithmetic!
    seed = (seed * multiplier % modulus) : uint(32);
    return (seed-1) / (modulus-2) : real;
  }

  var delay = RandomNumber (x) * 4.0;
  if delay < 2.0 then sleep (delay : uint);
  else for i in 1..(delay*10000.0) : int do
    delay = RandomNumber (x) * 4.0;
  total += x;
}

coforall i in 1..numThreads do
  foo (i);
writeln ("total is ", total.readFF());
