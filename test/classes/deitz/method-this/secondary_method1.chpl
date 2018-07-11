class foo {
  var i : int;
  proc primary() {
    writeln("primary method call");
  }
}

proc foo.secondary() {
  writeln("secondary method call");
}

var f : unmanaged foo = new unmanaged foo();

f.primary();
f.secondary();

delete f;
