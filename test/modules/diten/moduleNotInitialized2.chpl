var lock1: sync bool;
var lock2: sync bool;

module M1 {
  lock1 = false;
  lock2;
  var a = 2;
}

module M2 {
  def main {
    var b, c: sync int;
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
