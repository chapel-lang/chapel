proc foo() {
  var t = ("a", "b");
  proc bar() {
    var x = t(0);
    t(0) = t(1);
    t(1) = x;
  }
  writeln(t);
  bar();
  writeln(t);
}

foo();
