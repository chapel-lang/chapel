fun bar() {
  var x : int = 12;
  var y : int = 15;
  var z : int = 18;
  fun foo() {
    writeln(x);
    if (x > 0) {
      goo();
    }
  }
  fun goo() {
    writeln(y);
    if (y > 0) {
      fie();
    }
  }
  fun fie() {
    writeln(z);
    if (z < 0) {
      foo();
    }
  } 
  foo();
}
bar();
