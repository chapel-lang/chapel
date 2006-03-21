class foo {
  var x : int;
  function getx() : int {
    return x;
  }
}

var f : foo = foo();

f.x = 3;
writeln("the int is ", f.getx());
