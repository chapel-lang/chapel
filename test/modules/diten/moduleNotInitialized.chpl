module M1 {
  use Time;
  class C {
    var field: int;
  }
  sleep(3);
  var a = new C(1);
}

module M2 {
  def main {
    var b, c:object;
    begin {
      use M1;
      b = a;
    }
    use M1;
    c = a;
    writeln(b);
    writeln(c);
  }
}
