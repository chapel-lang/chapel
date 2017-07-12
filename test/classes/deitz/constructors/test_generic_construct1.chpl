class C {
  type ct;
  var x: ct;

  proc init(type ctVal, y: ctVal) {
    ct = ctVal;
    x = y;
    super.init();
  }
}

var c = new C(int, y=2);
writeln(c);
delete c;
