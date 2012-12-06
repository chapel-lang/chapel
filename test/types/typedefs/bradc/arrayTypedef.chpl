use BlockDist;

type ArrType = [1..8] real;
type ArrOfArr = [1..2] [1..3] real;
type DistArr = [{1..10} dmapped Block({1..10})] real;

proc main() {
  var A: ArrType;
  forall a in A do
    a = here.id;
  writeln("A is: ", A);

  var B: ArrOfArr;
  forall b in B do
    b = here.id;
  writeln("B is: ", B);

  var C: DistArr;
  forall c in C do
    c = here.id;
  writeln("C is: ", C);
}
