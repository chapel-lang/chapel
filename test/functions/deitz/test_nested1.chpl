function foo() {
  var x;
  x = 1;
  function bar() {
    var y;
    y = 2;
    writeln(x);
    writeln(y);
  }
  bar();
}

foo();
