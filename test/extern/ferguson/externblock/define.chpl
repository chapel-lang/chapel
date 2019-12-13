module OuterModule {
  module M {
    extern {
      #define TEST_DEFINE                               0x0007
    }
  }

  writeln(M.TEST_DEFINE);
}
