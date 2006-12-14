use Time;
use Memory;

var y: sync int;

var u = memoryUsed();

class C {
  var x: int;
}

def foo() {
  var c = C(2);

  begin {
    sleep(3);
    y = c.x;
  }

  return 1;
}

var t = foo();
var tt = y;

var uu = memoryUsed();

writeln((t, tt));
writeln(uu-u);
