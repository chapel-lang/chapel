module TestUnstableModule {
  @unstable("Outer is unstable")
  module Outer {
    var y: bool;

    module Inner {
      var x: int;
    }
  }

  proc main() {
    {
      import this.Outer.Inner; // Should trigger Outer's message

      writeln(Inner.x);

    }

    {
      import this.Outer; // Should trigger Outer's message, but only once

      writeln(Outer.y); // Should trigger Outer's message
    }
  }
}
