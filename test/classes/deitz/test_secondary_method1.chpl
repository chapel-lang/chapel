class foo {
  var i : int;
  fun primary() {
    writeln("primary method call");
  }
}

fun foo.secondary() {
  writeln("secondary method call");
}

var f : foo = foo();

f.primary();
f.secondary();
