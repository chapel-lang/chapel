class ParentCallInIf {
  var f1: int;

  proc init(param val: int) {
    if (val > 10) {
      f1 = val;
    } else {
      f1 = -val;
      f1 += 3;
    }
  }
}

proc main() {
  var c1: borrowed ParentCallInIf = new borrowed ParentCallInIf(7);
  var c2: borrowed ParentCallInIf = new borrowed ParentCallInIf(13);
  writeln(c1);
  writeln(c2);
}
