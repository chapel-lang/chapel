proc doit():int {
  writeln("In doit");
  return 15;
}

record R {
  var x:int = doit();
  proc init() {
    init this;
    writeln("R constructor");
    x = 15;
  }
  proc deinit() {
    writeln("R destructor");
  }
}

var r = new R();
writeln(r.x);

var r2:R;

writeln(r2.x);

