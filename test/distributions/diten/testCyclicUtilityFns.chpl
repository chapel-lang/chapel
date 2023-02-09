use CyclicDist;
var rng = (1..10, 1..10);
var D = {(...rng)};

var CD1 = Cyclic.createDomain(D);
var CA1 = Cyclic.createArray(D, int);
var CD2 = Cyclic.createDomain((...rng));
var CA2 = Cyclic.createArray((...rng), int);

printLocales(CD1);
writeln();
printLocales(CA1);
writeln();
printLocales(CD2);
writeln();
printLocales(CA2);

proc printLocales(D: domain) {
  var A: [D] int;
  forall i in D do A[i] = here.id;
  writeln(A);
}

proc printLocales(in A: [] int) {
  forall a in A do a = here.id;
  writeln(A);
}
