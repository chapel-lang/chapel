class TestClass {
  var i : int;
  pragma "export" proc extern_method(j:int) : int {
    return j + 3;
  }
}
