module M {
  proc foo() {
    writeln("In foo");
  }
}

module N {
  proc main() {
    use M only;
    M.foo();
  }
}
