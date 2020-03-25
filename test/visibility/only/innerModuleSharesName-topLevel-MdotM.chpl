module OuterModule {
  module M {
    module M {
      proc whatev() {
        writeln("whee");
      }
    }
  }

  use M only;
  M.M.whatev();
}
