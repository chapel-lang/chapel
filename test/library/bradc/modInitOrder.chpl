module M1 {
  use M8;
  writeln("In M1's initializer");

  module M2 {
    use M12;
    use M3;
    writeln("In M2's initializer");
    use M10;
    proc main() {
      use M7;
      writeln("In main");
    }
    use M13;
    use M3.M4;
    use M5.M6;
  }

  use M9;
}

module M3 {
  writeln("In M3's initializer");

  module M4 {
    writeln("In M4's initializer");
  }
}

module M5 {
  writeln("In M5's initializer");
  module M6 {
    writeln("In M6's initializer");
  }
}

module M7 {
  writeln("In M7's initializer");
}

module M8 {
  writeln("In M8's initializer");
}

module M9 {
  writeln("In M9's initializer");
}

module M10 {
  use M11;
  writeln("In M10's initializer");
}

module M11 {
  use M10;
  writeln("In M11's initializer");
}

module M12 {
  use M13;
  writeln("In M12's initializer");
}

module M13 {
  writeln("In M13's initializer");
}
