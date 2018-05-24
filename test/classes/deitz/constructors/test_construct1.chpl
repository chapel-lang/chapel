class C {
  var x: int;
  var y: int;

  proc init(xVal, yVal) {
    x = xVal;
    y = yVal;
  }

  proc init(b: bool) {
    this.complete();
    if b then
      x = 24;
    else
      y = 12;
  }
}

var c = new unmanaged C(2, 3);
writeln(c);
delete c;

c = new unmanaged C(true);
writeln(c);
delete c;
