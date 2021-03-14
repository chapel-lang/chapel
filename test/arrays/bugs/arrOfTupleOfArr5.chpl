proc foo() {
  return ([1,2,3], [1.2,3.4,5.6]);
}

var A: [1..2] foo().type;
writeln(A);
