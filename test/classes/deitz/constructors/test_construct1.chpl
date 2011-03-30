class C {
  var x: int;
  var y: int;
  proc C(b: bool) {
    if b then
      x = 24;
    else
      y = 12;
  }
}

var c = new C(2, 3);
writeln(c);
c = new C(true);
writeln(c);
