module Main {
  deprecated "x is deprecated, use y instead" var x: int = 17;
  var y: int = 16;

  deprecated module Deep {
    // To check that we squash deprecation warnings for deep uses
    module Deeper {
      proc foo() {
        use Main;
        writeln(x);
      }

      // Even deeper!
      module Deepest {
        proc bar() {
          use Main;
          writeln(x);
        }
      }
    }
  }

  proc main() {
    use this.Deep.Deeper;
    use this.Deep.Deeper.Deepest;

    foo();
    bar();
  }
}
