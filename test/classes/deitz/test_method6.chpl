class foo {
  var x : integer;
  function getx() : integer {
    return x;
  }
}

var f : foo = foo();

f.x = 3;
writeln("the integer is ", f.getx());
