config const side   = 5000000;
config const trials = 100;
config const n      = 10000;

const D = {1..side*side};
var S : sparse subdomain(D);

// Run serial once to verify that there aren't out-of-bounds or other issues.
test(true, 1);
writeln("Serial Success");
test(false, trials);
writeln("Parallel Success");

proc test(param doSerial : bool, trials : int) {
  for 1..trials {
    S.clear();
    serial doSerial {
      forall i in 1..n with (ref S) do
        S += i;
    }
    assert(S.size == n);
  }
}
