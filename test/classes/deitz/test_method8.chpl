class foo {
  var x : integer = 12;
  function bar(x : integer) {
    writeln(x);
  }
}

var f : foo = foo();

f.x = 12;
f.bar(13);
f.bar(f.x);
