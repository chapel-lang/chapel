class foo {
  var i : integer;
  function primary() {
    writeln("primary method call; i is ", i);
  }
  function setx() {
    i = 4;
  }
}

var f : foo = foo();

f.setx();
f.primary();
