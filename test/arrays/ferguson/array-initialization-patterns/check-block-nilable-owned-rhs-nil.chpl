use BlockDist;

class MyClass {
  var x: int;
}

proc main() {
  const Space = {1..2};
  const Dom1: domain(1) dmapped Block(boundingBox=Space) = Space;
  const Dom2: domain(1) dmapped Block(boundingBox=Space) = Space;
  var A: [Dom1] owned MyClass?;

  A[1] = new MyClass(1);
  A[2] = new MyClass(2);

  var B = A;
  writeln(A); // should be nils
  writeln(B);

  var C: [Dom1] owned MyClass? = B;
  writeln(B); // should be nils
  writeln(C);

  var D: [Dom2] owned MyClass? = C;
  writeln(C); // should be nils
  writeln(D);
}
