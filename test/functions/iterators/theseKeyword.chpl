// Different illegal uses in separate modules to avoid redefinition errors
module Mod1 {
  var these : int = 4;
}
module Mod2 {
  proc these() {
    return "these";
  }
}
module Mod3 {
  // free iter disallowed
  iter these() {
    yield 4;
  }
}
module Mod4 {
  record these { }
}
module Mod5 {
  class these {
    // non-iter method disallowed
    proc these() {
      return 4;
    }
  }
}
module Mod6 {
  module these { }
}
module Mod7 {
  class Foo {
    // iter method is the only allowed form
    iter these() {
      yield 4;
    }
  }
}
module Mod8 {
  class Foo { }
  // secondary method iter also allowed
  iter Foo.these() {
    yield 4;
  }
}
module Mod9 {
  module FooDecl {
    class Foo { }
  }
  // tertiary method iter also allowed
  import this.FooDecl.Foo;
  iter Foo.these() {
    yield 4;
  }
}
module Mod10 {
  class Foo {
    // also allow iterator-forwarding proc method
    pragma "fn returns iterator"
    proc these() {
      return this.these();
    }
    iter these() {
      yield 4;
    }
  }
}

// Using illegally-named symbols should not trigger warnings
module Main {
  import Mod1;

  proc main() {
    writeln(Mod1.these);
  }
}
