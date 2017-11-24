module M1 {
  use M2;
  use M4;

  proc main() {
    writeln('Hello world');
  }
}

module M2 {
  use M3;

  module M3 {
    module M4 {

    }
  }
}
