function foo.secondary() {
  writeln("secondary method call; i is ", i);
}

class foo {
  var i : integer = 4;
  function primary() {
    writeln("primary method call; i is ", i);
  }
}

var f : foo = foo();

f.primary();
f.secondary();
