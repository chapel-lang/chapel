module OuterModule {
  module A {
    extern {
      static int x;
    }
  }

  module B {
    extern {
      static int y;
    }
  }

  A.x = 3;
  B.y = 4;

  writeln(A.x);
  writeln(B.y);
}
