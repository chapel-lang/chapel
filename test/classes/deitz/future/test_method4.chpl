class foo {
  var x : integer;
  function setx(i : integer) {
    x = i;
  }
  function getx() : integer {
    return x;
  }
}

var f : foo = foo();

f.setx(3);
writeln(f.getx());
