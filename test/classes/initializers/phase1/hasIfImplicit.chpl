class IfInit {
  var f1: bool;
  var f2: int;

  proc init(val: int) {
    f1 = val < 5;

    if (!f1) {
      f2 = val;
    }

    // Error f2 is only initialized if f1 is false

  }
}

proc main() {
  var ownC1 = new owned IfInit(3);
  var c1 : borrowed IfInit = ownC1.borrow();

  writeln(c1);

  var ownC2 = new owned IfInit(7);
  var c2 = ownC2.borrow();
  writeln(c2);
}
