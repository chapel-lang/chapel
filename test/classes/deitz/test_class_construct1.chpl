class foo {
  var x : integer = 12;
  function initialize() {
    x += 10;
  }
}

var f : foo = foo(50);

writeln(f);
