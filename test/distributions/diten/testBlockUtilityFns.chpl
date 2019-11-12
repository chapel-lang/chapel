use BlockDist;
var rng = (1..10, 1..10);
var D = {(...rng)};

var BD1 = BlockDom.create(D);
var BA1 = BlockArr.create(D, int);
var BD2 = BlockDom.create((...rng));
var BA2 = BlockArr.create((...rng), int);

printLocales(BD1);
writeln();
printLocales(BA1);
writeln();
printLocales(BD2);
writeln();
printLocales(BA2);

proc printLocales(D: domain) {
  var A: [D] int;
  forall i in D do A[i] = here.id;
  writeln(A);
}

proc printLocales(in A: [] int) {
  forall a in A do a = here.id;
  writeln(A);
}
