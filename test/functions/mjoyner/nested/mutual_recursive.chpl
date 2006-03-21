function bar() {
  var x : int = 12;
  var y : int = 15;
  var z : int = 18;
  function foo() {
    writeln(x);
    if (x > 0) {
      goo();
    }
  }
  function goo() {
    writeln(y);
    if (y > 0) {
      fie();
    }
  }
  function fie() {
    writeln(z);
    if (z < 0) {
      foo();
    }
  } 
  foo();
}
bar();
