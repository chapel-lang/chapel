class foo {
  var x : integer;
  function setxlo(i : integer) {
    x = i-1;
  }
  function setxhi(i : integer) {
    x = i+1;
  }
}

var f : foo = foo();

f.setxhi(3);
writeln(f.x);
f.setxlo(3);
writeln(f.x);
