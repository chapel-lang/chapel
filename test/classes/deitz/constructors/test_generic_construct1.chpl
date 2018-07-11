class C {
  type ct;
  var x: ct;

  proc init(type ctVal, y: ctVal) {
    ct = ctVal;
    x = y;
  }
}

var c = new unmanaged C(int, y=2);
writeln(c);
delete c;
