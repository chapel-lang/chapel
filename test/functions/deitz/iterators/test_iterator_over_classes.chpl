class C {
  var x: int;
}

var c1 = C(1);
var c2 = C(2);
var c3 = C(3);

def foo() {
  yield c1;
  yield c2;
  yield c3;
}

for i in foo() do
  writeln(i);
