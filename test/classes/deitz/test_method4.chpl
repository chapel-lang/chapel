class foo {
  var x : int;
  function setx(i : int) {
    x = i;
  }
  function getx() : int {
    return x;
  }
}

var f : foo = foo();

f.setx(3);
writeln(f.getx());
