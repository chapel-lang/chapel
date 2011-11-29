proc main {
  var D: domain (1) = [1..10];
  var A: [D] real;
  writeln(chpldev_refToString(D));
  writeln(chpldev_refToString(A.domain));
  writeln(chpldev_refToString(A));
}
