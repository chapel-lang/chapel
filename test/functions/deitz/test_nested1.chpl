fun foo() {
  var x;
  x = 1;
  fun bar() {
    var y;
    y = 2;
    writeln(x);
    writeln(y);
  }
  bar();
}

foo();
