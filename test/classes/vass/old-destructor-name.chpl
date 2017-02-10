
class C {
  var x: int;
  proc C(arg: int) {
    x = arg;
    writeln("C(", x, ")");
  }
  proc ~C() {
    writeln("~C(", x, ")");
  }
}

record R {
  var y: int;
  proc R(arg: int) {
    y = arg;
    writeln("R(", y, ")");
  }
  proc ~R() {
    writeln("~R(", y, ")");
  }
}

var d: C;
writeln("start");
{
var c = new C(44);
var r = new R(55);
d = c;
}
writeln("done");
delete d;
