class C {
  var a: int = b+1;
  var b: int = a-1;
}

var obj = new C();
var c: borrowed C = obj.borrow();

writeln("a=", c.a, " b=", c.b);
