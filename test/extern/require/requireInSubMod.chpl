module M {
  require "foo.h";

  extern proc foo();

  module M2 {
    //
    // The following file intentionally contains syntax errors to make
    // sure we don't add requirements for modules that are not 'use'd.
    //
    require "bar.h";

    extern proc bar();
  }
}

module M2 {
  proc main() {
    use M;
    foo();
  }
}
