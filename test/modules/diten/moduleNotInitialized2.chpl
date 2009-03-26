module M1 {
  use Time;
  sleep(3);
  var a = 2;
}

module M2 {
  def main {
    var b, c:int;
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
