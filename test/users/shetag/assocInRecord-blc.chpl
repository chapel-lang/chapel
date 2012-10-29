record C {
  var d : domain(string);
  var a : [d] int;
}

var myC = new C();
var D = {1..4};
var A: [D] string = ("one", "two", "three", "four");

for i in D {
  myC.d += A(i);
  const Aval = A(i);
  myC.a(Aval) = i;
  for j in A[1..i] {
    writeln("A(", j, ") = ", myC.a(j));
  }
  writeln();
}
