module OuterModule {
  module A {
    extern {
      static int x;
    }
  }

  extern {
    static int y;
  }

  A.x = 3;
  y = 4;

  writeln(A.x);
  writeln(y);
}
