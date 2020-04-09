module Main {
  use M1;
  use M2;
  use M3;
  use M4;

  writeln();
  writeln('M.1: Module Main');

  proc main() {
    writeln();
    writeln('Hello');
  }
}



module M1 {
  use M2;
  use M3;
  use M4;

  writeln('1.6: Module M1');
  writeln();

  module M2 {
    writeln('1.1: Module M1.M2');
  }
}



module M2 {
  use M1;
  use M3;
  use M4;

  writeln('2.2: Module M2');

  module M1 {
    writeln('2.1: Module M2.M1');
  }
}



module M3 {
  use M1;
  use M2;
  use M4;

  writeln('1.5: Module M3');

  module M2 {
    writeln('1.2: Module M3.M2');
  }
}



module M4 {
  use M1;
  use M2;
  use M3;

  writeln('1.4: Module M4');

  module M3 {
    writeln('1.3: Module M4.M3');
  }
}
