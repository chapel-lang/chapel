module OuterModule {
  var y = 3;
  class D {
    class C {
      var x = 2;
    }
  }

  module M {
    var y = 1;

    proc main() {
      var y = 4;
      proc C.foo() {
        writeln(y, x);
      }
      var c = new C();
      c.foo();
    }
  }
}
