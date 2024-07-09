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
  var ownC1 = new owned ParentCallInIf(7);
  var ownC2 = new owned ParentCallInIf(13);
  var c1: borrowed ParentCallInIf = ownC1.borrow();
  var c2: borrowed ParentCallInIf = ownC2.borrow();
  writeln(c1);
  writeln(c2);
}
