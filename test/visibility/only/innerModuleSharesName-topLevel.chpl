module OuterModule {
  module M {
    module M {
      proc whatev() {
        writeln("whee");
      }
    }
  }

  use this.M only M;
  M.whatev();
}
