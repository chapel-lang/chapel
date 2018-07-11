pragma "safe"
module TestReturningIteratorYield {


  var global0 = 0;
  var global1 = 1;
  var global2 = 2;
  var global3 = 3;
  iter myiter() ref {
    yield global1;
    yield global2;
    yield global3;
  }

  proc getfirstref() ref {
    for i in myiter() {
      return i;
    }
    return global0;
  }
  proc test() {
    ref x = getfirstref();
    writeln(x);
  }
  test();
}
