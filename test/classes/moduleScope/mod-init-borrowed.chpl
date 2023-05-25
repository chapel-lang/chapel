module OuterModule {
  proc main() {
    var a   = (new owned A(10)).borrow();

    writeln();

    var m1a = (new owned M1.A(20)).borrow();

    writeln();

    var m2a = new M2.A(40);
  }

  class A {
    proc init(a : int) {
      writeln("A.init(", a, ")");
    }
  }

  // A module with a class with an initializer
  module M1 {
    class A {
      proc init(a : int) {
        writeln("M1.A.init(", a, ")");
      }
    }
  }


  // A module with a record with an initializer
  module M2 {
    record A {
      proc init(a : int) {
        writeln("M2.A.init(", a, ")");
      }
    }
  }
}
