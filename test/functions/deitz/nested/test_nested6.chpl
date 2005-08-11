function bar() {
  var x : integer = 12;
  var y : integer = 15;
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
