use BlockDist;

config const n = 1000;

var D = {1..n} dmapped new blockDist({1..n});

serial {
  forall i in D do
    writeln(i);
}
