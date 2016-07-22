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
  var c1: SiblingCallInIf = new SiblingCallInIf(3);
  var c2: SiblingCallInIf = new SiblingCallInIf(12);
  writeln(c1);
  writeln(c2);
  delete c1;
  delete c2;
}
