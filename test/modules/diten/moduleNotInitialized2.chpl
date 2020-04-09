module OuterModule {
  var lock1: sync bool;
  var lock2: sync bool;

  module M1 {
    use OuterModule;
    var a: int;
    var raninit = false;
    proc init() {
      if (!raninit) {
        raninit = true;
        lock1 = false;
        lock2;
        a = 2;
      }
    }
  }

  module M2 {
    use OuterModule;
    proc main {
      var b, c: sync int;
      begin {
        use M1;
        M1.init();
        b = a;
      }
      lock1;
      use M1;
      M1.init();
      c = a;
      lock2 = false;
      writeln(b.readFF());
      writeln(c.readFF());
    }
  }
}
