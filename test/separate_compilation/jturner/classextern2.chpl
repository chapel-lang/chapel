class TestClass {
  var i : int;
}
export proc extern_method(foo:TestClass) : int {
  return foo.i + 1;
}
