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
  var c1: borrowed ParentCallInIf = (new owned ParentCallInIf(7)).borrow();
  var c2: borrowed ParentCallInIf = (new owned ParentCallInIf(13)).borrow();
  writeln(c1);
  writeln(c2);
}
