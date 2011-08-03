class TestClass {
  var i : int;
}
pragma "export" proc extern_method(foo:TestClass, i:int) : int {
  return i + 1;
}
