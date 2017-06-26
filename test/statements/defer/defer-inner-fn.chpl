proc test() {
  var x = 1;
  defer {
    proc inner() {
      x = 2;
      return;
    }
    inner();
    writeln("in defer after inner(), x is ", x);
  }
  writeln("in test, x is ", x);
}


test();
