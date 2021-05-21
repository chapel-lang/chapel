module Main {
  deprecated "This module is deprecated, use Foob instead"
  module Foo {
    public use super.Foob;
  }

  module Foob {
    var x: int = 3;

    proc bar() {
      writeln("In Foob.bar()");
    }
  }

  proc main() {
    {
      use Foo;        // Should trigger the warning
      writeln(x);     // Should not trigger the warning
      bar();          // Should not trigger the warning
      writeln(Foo.x); // Should trigger the warning
      Foo.bar();      // Should trigger the warning
    }

    {
      use Foo only x; // Should trigger the warning
      writeln(x);     // Should not trigger the warning
      writeln(Foo.x); // Should trigger the warning
    }

    {
      use Foo except x; // Should trigger the warning
      Foo.bar();        // Should trigger the warning
      bar();            // Should not trigger the warning
    }

    {
      import this.Foo;     // Should trigger the warning
      writeln(Foo.x); // Should trigger the warning
      Foo.bar();      // Should trigger the warning
    }

    {
      import this.Foo.x; // Should trigger the warning
      writeln(x);   // Should not trigger the warning
    }

    {
      import this.Foo.bar; // Should trigger the warning
      bar();          // Should not trigger the warning
    }
  }
}
