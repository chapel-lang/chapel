proc zero(A : [?D] real) {
  forall i in D {
    A(i) = 0;
  }
}

var a1 : [1..10] real;
var a2 : [1..10, 1..10] real;

zero(a1);
zero(a2);
writeln(a1(1));
writeln(a2(1,1));
