module OuterModule {
  // This test confirms that functions in another module can be called
  // using an explicit module name
  //

  module M1 {
    proc version1   : (int, int, int) {
      return (1, 2, 3);
    }

    proc version2() : (int, int, int) {
      return (4, 5, 6);
    }

    proc version3(a : int, b : int, c : int) : (int, int, int) {
      return (a, b, c);
    }

  }

  module M2 {
    proc version1   : (int, int, int) {
      return (11, 12, 13);
    }

    proc version2() : (int, int, int) {
      return (14, 15, 16);
    }

    proc version3(a : int, b : int, c : int) : (int, int, int) {
      return (a, b, c);
    }
  }



  use M1;

  proc main() {
    testMod1();
    writeln();
    writeln();

    testMod2();
  }

  proc testMod1() {
    var (A, B, C) = M1.version1;
    var (D, E, F) = M1.version2();
    var (G, H, I) = M1.version3(7, 8, 9);

    writeln(A, ' ', B, ' ', C);
    writeln(D, ' ', E, ' ', F);
    writeln(G, ' ', H, ' ', I);


  }


  proc testMod2() {
    var (A, B, C) = M2.version1;
    var (D, E, F) = M2.version2();
    var (G, H, I) = M2.version3(17, 18, 19);

    writeln(A, ' ', B, ' ', C);
    writeln(D, ' ', E, ' ', F);
    writeln(G, ' ', H, ' ', I);
  }

}
