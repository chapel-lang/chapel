class ParentCallInIf {
  var f1: int;

  proc init(param val: int) {
    if (val > 10) {
      f1 = val;
    } else {
      this.complete();
      f1 = -val;
      f1 += 3;
      // Because the conditional is param, this section will know it is in Phase
      // 2
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
