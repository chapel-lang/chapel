module OuterModule {
  var lock1: sync bool;
  var lock2: sync bool;

  module M1 {
    use OuterModule;
    var a: int;
    var raninit = false;
    proc M1init() {
      if (!raninit) {
        raninit = true;
        lock1.writeEF(false);
        lock2.readFE();
        a = 2;
      }
    }
  }

  module M2 {
    use OuterModule;
    proc main {
      var b, c: sync int;
      begin with (ref a) {
        use M1;
        M1init();
        b.writeEF(a);
      }
      lock1.readFE();
      use M1;
      M1init();
      c = a;
      lock2.writeEF(false);
      writeln(b.readFF());
      writeln(c.readFF());
    }
  }
}
