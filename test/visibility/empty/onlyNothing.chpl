config param qualifiedAccess = false;

module M {
  proc foo() {
    writeln("In foo");
  }
}

module M2 {
  use M only ;   // require all symbols in M to be fully-qualified

  proc main() {
    if qualifiedAccess then
      M.foo();
    else
      foo();
  }
}
