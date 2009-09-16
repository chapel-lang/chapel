module M1 {
  writeln("In M1's initializer");

  module M2 {
    use M3;
    writeln("In M2's initializer");
    def main() {
      use M7;
      writeln("In main");
    }
    use M3.M4;
    use M5.M6;
  }

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
    writeln("In m6's initializer");
  }
}

module M7 {
  writeln("In M7's initizlier");
}
