module M1 {
  class M1 {
  }
}

module M2 {
  //  use M1;
}

module M3 {
  use M2;
  use M1;
  proc main {
    writeln("hello world");
  }
}
