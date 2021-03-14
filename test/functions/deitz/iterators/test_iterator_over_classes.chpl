class C {
  var x: int;
}

var c1 = new unmanaged C(1);
var c2 = new unmanaged C(2);
var c3 = new unmanaged C(3);

iter foo() {
  yield c1;
  yield c2;
  yield c3;
}

for i in foo() do
  writeln(i);

delete c1;
delete c2;
delete c3;
