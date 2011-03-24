class TestClass {
  var i : int;
}
pragma "export" proc extern_method(foo:TestClass) : int {
  return foo.i + 1;
}
