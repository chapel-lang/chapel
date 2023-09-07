use BlockDist;
var rng = (1..10, 1..10);
var D = {(...rng)};

var BD1 = blockDist.createDomain(D);
var BA1 = blockDist.createArray(D, int);
var BD2 = blockDist.createDomain((...rng));
var BA2 = blockDist.createArray((...rng), int);

printLocales(BD1);
writeln();
printLocales(BA1);
writeln();
printLocales(BD2);
writeln();
printLocales(BA2);

proc printLocales(D: domain) {
  var A: [D] int;
  forall i in D with (ref A) do A[i] = here.id;
  writeln(A);
}

proc printLocales(in A: [] int) {
  forall a in A do a = here.id;
  writeln(A);
}
