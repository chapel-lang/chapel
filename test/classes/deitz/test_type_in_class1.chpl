class foo {
  class bar {
    var x : integer;
    var y : integer;
  }
  var b : bar = bar();
  var z : integer;
}

var f : foo = foo();

f.b.x = 1;
f.b.y = 2;
f.z = 3;

writeln(f.b.x, f.b.y, f.z);
