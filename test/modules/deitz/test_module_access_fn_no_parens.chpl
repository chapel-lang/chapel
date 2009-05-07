module M1 {
  def f return 3;
}

module M2 {
  use M1;
  def main {
    var f = 4;
    writeln(f);
    writeln(M1.f);
  }
}
