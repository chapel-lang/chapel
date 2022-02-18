module M {
  require "foo.h";

  extern proc foo();
  foo();
}

module M2 {
  proc main() {
    writeln("In main()");
  }
}
