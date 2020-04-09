module M {
  //
  // The following file intentionally contains syntax errors to make
  // sure we don't add requirements for modules that are not 'use'd.
  //
  require "bar.h";

  extern proc bar();
}

module M2 {
  proc main() {
    writeln("In main()");
  }
}
