class TestClass {
  var x:int;
}

proc testfun() {
  var c = new unmanaged TestClass(x=2);
  return c;
}

var f = testfun;

var d = f();
writeln(d.x);

delete d;
