use BlockDist;

type DistArr = [{1..10} dmapped Block({1..10})] real;

proc main() {
  var A: DistArr;

  forall a in A do
    a = here.id;
  writeln("A is: ", A);
}
