module M1 {
  private var foo = 15;

  module M2 {
    proc main() {
      use M1;
      __primitive(c"get visible symbols", ignoreBuiltinModules=true);
      // Should print foo, because we are under the
      // parent of the private variable.
    }
  }
}
