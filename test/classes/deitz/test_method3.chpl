class foo {
  var x : int;
  function setxlo(i : int) {
    x = i-1;
  }
  function setxhi(i : int) {
    x = i+1;
  }
}

var f : foo = foo();

f.setxhi(3);
writeln(f.x);
f.setxlo(3);
writeln(f.x);
