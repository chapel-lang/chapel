use BlockDist;

var D1 = {1..10} dmapped blockDist;
displayDist(D1);

proc displayDist(Dom) {
  var A: [Dom] int;

  forall a in A do
    a = here.id;

  writeln(A);
}
