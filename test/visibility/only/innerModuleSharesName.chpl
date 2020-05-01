module OuterModule {
  module EvenMoreOuter {
    module M {
      module M {
        proc whatev() {
          writeln("whee");
        }
      }
    }
  }

  use EvenMoreOuter.M only M;
  M.whatev();
}
