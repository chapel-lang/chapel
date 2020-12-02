module OuterModule {
  var lock1: sync bool;
  var lock2: sync bool;

  module M1 {
    use OuterModule;
    class C {
      var field: int;
    }
    var a: unmanaged C?;
    var raninit = false;
    proc M1init() {
      if (!raninit) {
        raninit = true;
        lock1 = false;
        lock2;
        a = new unmanaged C(1);
      }
    }
  }

  module M2 {
    use OuterModule;
    proc main {
      var b, c: sync borrowed object?;
      begin with (ref a) {
        use M1;
        M1init();
        b = a;
      }
      lock1;
      use M1;
      M1init();
      c = a;
      lock2 = false;

      writeln(b.readFF());
      writeln(c.readFF());
      delete M1.a;
    }
  }
}
