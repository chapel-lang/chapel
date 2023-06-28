class UsesHelpers {
  var f1: bool;
  param f2: int;
  var f3: int;

  proc init(param val: int) {
    f1 = helperFunc(val);
    param helper = val + 3;
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
  var ownC = new owned UsesHelpers(9);
  var c = ownC.borrow();
  writeln(c.type: string);
  writeln(c);
}
