class foo {
  var i : int;
  def primary() {
    writeln("primary method call");
  }
}

def foo.secondary() {
  writeln("secondary method call");
}

var f : foo = foo();

f.primary();
f.secondary();
