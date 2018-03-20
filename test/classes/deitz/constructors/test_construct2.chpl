class C {
  var x: int;
  var y: int;
  proc init(b: bool) {
    this.complete();
    if b then
      x = 24;
    else
      y = 12;
  }
}

var c = new C(true);
writeln(c);
delete c;
