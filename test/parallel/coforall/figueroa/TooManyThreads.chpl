use Time;

config const numThreads = 8000;
var total: sync int = 0;
var seed = getCurrentTime() : uint;

def foo (x) {

  def RandomNumber (y) {
    const multiplier: int(64) = 16807,
          modulus: int(64) = 2147483647;
    // The following calculation must be done in at least 46-bit arithmetic!
    seed = (seed * multiplier % modulus) : uint;
    return (seed-1) / (modulus-2) : real;
  }

  var delay = RandomNumber (x) * 10.0;
  //writeln (x, ": sleeping for ", delay, " seconds");
  if delay < 9.0 then sleep (delay : uint + 4);
  else for i in 1..(delay*100000.0) : int do
    delay = RandomNumber (x) * 4.0;
  total += x;
}

coforall i in 1..numThreads do
  foo (i);
writeln ("total is ", total);
