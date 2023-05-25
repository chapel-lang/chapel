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
  var c1: borrowed SiblingCallInIf = (new owned SiblingCallInIf(3)).borrow();
  var c2: borrowed SiblingCallInIf = (new owned SiblingCallInIf(12)).borrow();
  writeln(c1);
  writeln(c2);
}
