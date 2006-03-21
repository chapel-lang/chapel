class foo {
  var i : int;
  fun primary() {
    writeln("primary method call; i is ", i);
  }
  fun setx() {
    i = 4;
  }
}

var f : foo = foo();

f.setx();
f.primary();
