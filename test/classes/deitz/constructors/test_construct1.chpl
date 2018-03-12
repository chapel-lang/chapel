class C {
  var x: int;
  var y: int;

  proc init(xVal, yVal) {
    x = xVal;
    y = yVal;
  }

  proc init(b: bool) {
    this.initDone();
    if b then
      x = 24;
    else
      y = 12;
  }
}

var c = new C(2, 3);
writeln(c);
delete c;

c = new C(true);
writeln(c);
delete c;
