class ParentCallInIf {
  var f1: int;

  proc init(param val: int) {
    if (val > 10) {
      f1 = val;
      super.init();
    } else {
      f1 = -val;
      super.init();
      f1 += 3;
    }
  }
}

proc main() {
  var c1: ParentCallInIf = new ParentCallInIf(7);
  var c2: ParentCallInIf = new ParentCallInIf(13);
  writeln(c1);
  writeln(c2);
  delete c1;
  delete c2;
}
