class ManyFields {
  param f4 = f5 * 4;
  param f5 = 3;

  proc init(param val: int) {
    // f4's implicit reliance on f5 is bad, but the field declaration will tell
    // us this.
    f5 = val;
    this.complete();
    // The proper way to write this is in Phase 2, or by relying on the val
    // argument.  You may also choose to initialize these fields with noinit
    // during Phase 1
  }
}

proc main() {
  var c = new borrowed ManyFields(2);
  writeln(c.type: string);
  delete c;
}
