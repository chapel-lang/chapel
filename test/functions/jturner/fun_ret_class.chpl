class TestClass {
  var x:int;
}

proc testfun() {
  var c = new TestClass(x=2);
  return c;
}

var f = testfun;

var d = f();
writeln(d.x);