class TestClass {
  var i : int;
  proc extern_method() : int;
}

proc main() {
  var c = new TestClass();
  c.i = 10;
  writeln(c.extern_method());
}
