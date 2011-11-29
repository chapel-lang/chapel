class C {
  type ct;
  var x: ct;

  proc C(type ct, y: ct) {
    x = y;
  }
}

var c = new C(int, y=2);
writeln(c);
delete c;
