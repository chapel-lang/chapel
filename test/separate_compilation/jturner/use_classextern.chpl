class TestClass {
  var i : int;
  proc extern_method(i:int) : int;
}

proc main() {
  var c = new TestClass();
  c.i = 10;
  var x = c.i;
  writeln(c.extern_method(x));
}
