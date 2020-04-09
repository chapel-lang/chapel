proc foo() {
  record bar {
    var x : int = 2;
  }
  var b : bar;
  writeln(b.x);
}

foo();
