module A {
  module B {
    module Inner {
      var x: int;
    }
  }
}

module C {
  module Inner {
    var x: bool;
  }
}

module User {
  import A.B as C;
  import C.Inner; // Should this choose A.B.Inner (line 3)?
  // Or C.Inner (line 10)?  Fail? (Rust chooses A.B.Inner)

  proc main() {
    // Should this be a bool or an int?  Currently gives an int, and that
    // matches Rust.
    writeln(Inner.x);
  }
}
