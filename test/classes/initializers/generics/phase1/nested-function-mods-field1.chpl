// This test exercises the case where an initializer defines a function within
// itself, but the function modifies a field.
class Foo {
  param field: int;

  proc init(param val) {
    field = val;
    nested(); // This call shouldn't be allowed in Phase 1
    proc nested() {
      field = -field; // because this modifies a field
    }
  }
}

proc main() {
  var f = new borrowed Foo(13);
  writeln(f.type: string);
}
