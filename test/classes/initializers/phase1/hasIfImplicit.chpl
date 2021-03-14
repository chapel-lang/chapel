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
  var c1 : borrowed IfInit = new borrowed IfInit(3);

  writeln(c1);

  var c2 = new borrowed IfInit(7);
  writeln(c2);
}
