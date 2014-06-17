class foo {
  var i : int;
  proc primary() {
    writeln("primary method call; i is ", i);
  }
  proc setx() {
    i = 4;
  }
}

var f : foo = new foo();

f.setx();
f.primary();
