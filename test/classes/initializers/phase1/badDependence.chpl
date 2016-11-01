class ManyFields {
  var f2: int;
  var f3: int;
  var f4 = 12;
  var f5 = 3;

  proc init(val: int) {
    // f1, though omitted, has a dependence on a later field.  Giving it the
    // declaration value then violates the rules of phase 1
    f2 = f5 + 4; // Depends on a later field's explicit initialization.  Uh oh!
    f3 = f4 - 2; // Depends on an omitted later field.  Uh oh!
    // f4's implicit reliance on f5 is also bad
    f5 = val;
    super.init();
    // The proper way to write this is in Phase 2, or by relying on the val
    // argument.  You may also choose to initialize these fields with noinit
    // during Phase 1
  }
}

proc main() {
  var c: ManyFields = new ManyFields(2);
  writeln(c);
  delete c;
}
