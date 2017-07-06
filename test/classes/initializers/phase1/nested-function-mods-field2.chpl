// This test exercises the case where an initializer defines a function within
// itself, but the function modifies a field.
class Foo {
  var field: int;

  proc init(val) {
    proc nested() {
      field = -field; // This modifies a field
    }

    field = val;
    nested(); // so this call shouldn't be allowed in Phase 1
    super.init();
  }
}

proc main() {
  var f = new Foo(13);
  writeln(f);
  delete f;
}
