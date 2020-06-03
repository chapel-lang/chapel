module OuterModule {
  module MyCModule {
    extern {
      static int foo(int b) { return b + 1; }
    }
  }

  writeln("Hello");
  writeln(MyCModule.foo(7));

}
