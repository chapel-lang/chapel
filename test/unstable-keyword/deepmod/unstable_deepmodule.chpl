module TestUnstableModule {
  @unstable("x is unstable, use y instead") var x: int = 0;
  var y: int = 1;

  @unstable module TestUnstableModule2 {
    // checks deep uses
    module Deeper {
      proc foo() {
        use TestUnstableModule;
        writeln(x);
      }

      // Even deeper!
      module Deepest {
        proc bar() {
          use TestUnstableModule;
          writeln(x);
        }
      }
    }
  }

  proc main() {
    use this.TestUnstableModule2.Deeper;
    use this.TestUnstableModule2.Deeper.Deepest;

    foo();
    bar();
  }
}
