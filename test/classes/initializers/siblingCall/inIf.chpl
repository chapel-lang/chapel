class SiblingCallInIf {
  var alwaysDoubleDigits: int;
  var changed: bool;

  proc init(param num: int) {
    if (num >= 10) {
      this.init(num, false);
    } else {
      this.init(10, true);
    }
  }

  proc init(num: int, modified: bool) {
    alwaysDoubleDigits = num;
    changed = modified;
  }
}

proc main() {
  var c1Own = new owned SiblingCallInIf(3);
  var c1: borrowed SiblingCallInIf = c1Own.borrow();
  var c2Own = new owned SiblingCallInIf(12);
  var c2: borrowed SiblingCallInIf = c2Own.borrow();
  writeln(c1);
  writeln(c2);
}
