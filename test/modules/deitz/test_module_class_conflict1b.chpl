module MC {
  class MC {
    var x: int;
  }
}

module M1 {
  use MC except *;
  class C: MC {
    var y: int;
  }

  proc main {
    var c = new unmanaged C();
    writeln(c);
    delete c;
  }
}
