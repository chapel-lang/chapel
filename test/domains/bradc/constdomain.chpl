config var n: integer = 16;

function main() {
  const D: domain(1) = [1..n];
  var A: [D] integer;

  forall i in D {
    A(i) = i;
  }

  writeln("A is: ", A);
}