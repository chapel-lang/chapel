var lock1: sync bool;
var lock2: sync bool;

module M1 {
  class C {
    var field: int;
  }
  lock1 = false;
  lock2;
  var a = new C(1);
}

module M2 {
  def main {
    var b, c: sync object;
    begin {
      use M1;
      b = a;
    }
    lock1;
    use M1;
    c = a;
    lock2 = false;

    writeln(b);
    writeln(c);
  }
}
