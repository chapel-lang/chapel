class IfInit {
  param f1: bool;
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
  var c1 : IfInit(true, 0) = new IfInit(3);

  writeln(c1.type: string);

  delete c1;
}
