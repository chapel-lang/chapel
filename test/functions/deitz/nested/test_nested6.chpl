function bar() {
  var x : int = 12;
  var y : int = 15;
  function foo() {
    writeln(x);
    if (x > 0) {
      goo();
    }
  }
  function goo() {
    writeln(y);
    if (y < 0) {
      foo();
    }
  }
  foo();
}
bar();
