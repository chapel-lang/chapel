class UsesHelpers {
  var f1: bool;
  var f2: int;
  var f3: int;

  proc init(val: int) {
    f1 = helperFunc(val);
    var helper = val + 3;
    // The proper way to depend upon a later field is to utilize the intended
    // value for it.
    f2 = helper;
    f3 = val;
    this.complete();
    if (helper > 10) {
      writeln("double digits, woo!");
    }
  }

}

proc helperFunc(val: int) {
  return val > 3;
}

proc main() {
  var c: borrowed UsesHelpers = new borrowed UsesHelpers(9);
  writeln(c);
}
