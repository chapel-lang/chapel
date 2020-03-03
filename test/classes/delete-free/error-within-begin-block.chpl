class C { var x: int; }

proc test() {
  var bb: borrowed C?;
  sync begin with (ref bb) {
    var a = new owned C(1);
    bb = a.borrow();
  }
  writeln(bb!.x);
}
test();
