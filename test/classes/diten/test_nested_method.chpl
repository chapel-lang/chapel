module OuterModule {
  var w = 1; // There's a semantic question about if this should be accessible
             // through the foo method defined in main. If the answer to that
             // is no, remove w and the use of w in foo.
  class C {
    var x = 2;
  }

  module M {
    import OuterModule.C;
    var y = 3;

    proc main() {
      var z = 4;
      proc C.foo() {
        writeln(w, x, y, z);
      }
      var c = new unmanaged C();
      c.foo();
      delete c;
    }
  }
}
