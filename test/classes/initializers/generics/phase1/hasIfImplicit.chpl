class IfInit {
  var f1: bool;
  param f2: int;

  proc init(param val: int) {
    f1 = val < 5;

    if (!f1) {
      f2 = val;
    }

    // Error f2 is only initialized if f1 is false

  }
}

proc main() {
  var c1 : IfInit(3) = new IfInit(3);

  writeln(c1);

  delete c1;
}
