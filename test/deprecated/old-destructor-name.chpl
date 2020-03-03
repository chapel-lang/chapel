
class C {
  var x: int;
  proc init(arg: int) {
    x = arg;
    writeln("C(", x, ")");
  }
  proc ~C() {
    writeln("~C(", x, ")");
  }
}

record R {
  var y: int;
  proc init(arg: int) {
    y = arg;
    writeln("R(", y, ")");
  }
  proc ~R() {
    writeln("~R(", y, ")");
  }
}

var d: unmanaged C?;
writeln("start");
{
var c = new unmanaged C(44);
var r = new R(55);
d = c;
}
writeln("done");
delete d;
