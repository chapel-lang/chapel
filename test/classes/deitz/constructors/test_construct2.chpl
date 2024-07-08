class C {
  var x: int;
  var y: int;
  proc init(b: bool) {
    init this;
    if b then
      x = 24;
    else
      y = 12;
  }
}

var c = new unmanaged C(true);
writeln(c);
delete c;
