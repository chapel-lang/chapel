class TestClass {
  var i : int;
  pragma "export" proc extern_method() : int {
    return i + 1;
  }
}
