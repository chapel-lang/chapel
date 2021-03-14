class foo {
  class bar {
    var x : int;
    var y : int;
  }

  var b : unmanaged bar = new unmanaged bar();
  var z : int;

  proc deinit() {
    delete b;
  }
}

var f : unmanaged foo = new unmanaged foo();

f.b.x = 1;
f.b.y = 2;
f.z = 3;

writeln(f.b.x, f.b.y, f.z);

delete f;
