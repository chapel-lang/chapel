module MC {
  class MC {
    var x: int;
  }
}

module M2 {
  use MC;
}

module M1 {
  use M2;
  use MC;
  class C: MC {
    var y: int;
  }

  def main {
    var c = C();
    writeln(c);
  }
}
