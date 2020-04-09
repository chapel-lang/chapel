module OuterModule {
  module A {
    extern {
      static int x;
    }
  }

  extern {
    static int y;
  }

  x = 3;
  y = 4;

  writeln(x);
  writeln(y);
}
