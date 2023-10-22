use BlockDist;

config const n = 10,
             debug = false;

var D = {1..n} dmapped new blockDist(rank=1);
foo(D);

var D2 = {1..n} dmapped new blockDist(Locales);
foo(D2);

proc foo(Dom) {
  var A: [D] int;

  forall a in A do
    a = here.id;

  writeln(A);

  if debug then
    writeln(D.distribution);
}
