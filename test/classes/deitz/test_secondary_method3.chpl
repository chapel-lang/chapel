fun foo.secondary() {
  writeln("secondary method call");
}

class foo {
  var i : int;
  fun primary() {
    writeln("primary method call");
  }
}

var f : foo = foo();

f.primary();
f.secondary();
