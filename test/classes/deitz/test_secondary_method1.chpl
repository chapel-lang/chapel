class foo {
  var i : int;
  proc primary() {
    writeln("primary method call");
  }
}

proc foo.secondary() {
  writeln("secondary method call");
}

var f : foo = new foo();

f.primary();
f.secondary();
