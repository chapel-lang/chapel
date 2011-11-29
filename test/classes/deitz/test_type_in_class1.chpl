class foo {
  class bar {
    var x : int;
    var y : int;
  }
  var b : bar = new bar();
  var z : int;
}

var f : foo = new foo();

f.b.x = 1;
f.b.y = 2;
f.z = 3;

writeln(f.b.x, f.b.y, f.z);
