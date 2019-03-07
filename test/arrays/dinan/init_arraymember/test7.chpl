proc f() {
  var x: [1..10] real = 5.0;
  return x;
}

proc testVarTypeExpr() {
  var x: f().type = f();
  writeln(x);
}
testVarTypeExpr();

class C {
  var x: f().type = f();
}

proc testFieldTypeExpr() {
  var c = new unmanaged C();
  writeln(c.x);
  delete c;
}
testFieldTypeExpr();
