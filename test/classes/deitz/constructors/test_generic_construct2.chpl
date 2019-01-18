class C {
  type ct;
  type bt = real;
  var x: ct;
  var y: bt;

  proc init(type ct, type bt, y: ct) {
    this.ct = ct;
    this.bt = bt;
    x = y;
  }

  proc init(type ct, y: ct) {
    this.ct = ct;
    x = y;
  }
}

var c = new unmanaged C(int, y=2);
writeln(c);
var d = new unmanaged C(int, int, y=2);
writeln(d);
delete c;
delete d;
