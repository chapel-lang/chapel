function foo.secondary() {
  writeln("secondary method call; i is ", i);
}

class foo {
  var i : integer;
  function primary() {
    writeln("primary method call; i is ", i);
  }
}

var f : foo = foo();
f.i = 4;

f.primary();
f.secondary();
