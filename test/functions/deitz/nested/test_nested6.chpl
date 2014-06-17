proc bar() {
  var x : int = 12;
  var y : int = 15;
  proc foo() {
    writeln(x);
    if (x > 0) {
      goo();
    }
  }
  proc goo() {
    writeln(y);
    if (y < 0) {
      foo();
    }
  }
  foo();
}
bar();
