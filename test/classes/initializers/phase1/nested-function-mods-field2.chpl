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
  }
}

proc main() {
  var f = new unmanaged Foo(13);
  writeln(f);
  delete f;
}
