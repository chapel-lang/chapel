proc main() {
  var x: int;
  proc foo() {
    var y: int;
    proc bar() {
      x = 1;
      y = 2;
    }
    bar();
  }
  atomic foo();
  writeln("x = ", x);
}
