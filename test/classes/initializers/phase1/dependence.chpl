class ManyFields {
  var f1 = 3;
  var f2 = f1 * 4; // will have an implicit definition in the initializer
  var f3: int;
  var f4: int;
  var f5 = -f1;

  proc init(val: int) {
    f1 = val;
    // f2's initial value should be updated because of the change to f1, even
    // though it isn't explicitly re-stated
    f3 = f2 - 2; // Depends on an omitted field
    f4 = f1 + 4; // Depends on an earlier field's explicit initialization
  }
}

proc main() {
  var c: unmanaged ManyFields = new unmanaged ManyFields(2);
  writeln(c);
  delete c;
}
