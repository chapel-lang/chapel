proc bar() {
  var x : int = 12;
  var y : int = 15;
  var z : int = 18;
  proc foo() {
    writeln(x);
    if (x > 0) {
      goo();
    }
  }
  proc goo() {
    writeln(y);
    if (y > 0) {
      fie();
    }
  }
  proc fie() {
    writeln(z);
    if (z < 0) {
      foo();
    }
  } 
  foo();
}
bar();
