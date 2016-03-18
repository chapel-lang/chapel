proc foo.secondary() {
  writeln("secondary method call; i is ", i);
}

class foo {
  var i : int;
  proc primary() {
    writeln("primary method call; i is ", i);
  }
}

var f : foo = new foo();
f.i = 4;

f.primary();
f.secondary();
