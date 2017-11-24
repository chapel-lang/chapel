var lock1: sync bool;
var lock2: sync bool;

module M1 {
  class C {
    var field: int;
  }
  var a: C;
  var raninit = false;
  proc init() {
    if (!raninit) {
      raninit = true;
      lock1 = false;
      lock2;
      a = new C(1);
    }
  }
}

module M2 {
  proc main {
    var b, c: sync object;
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
    delete M1.a;
  }
}
