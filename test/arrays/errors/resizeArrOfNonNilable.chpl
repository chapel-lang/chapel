class C {
  var x: int;
}
var D = {1..3};
var A = [i in D] new C(x=i);
writeln(A);
D = {1..4};
writeln(A);
