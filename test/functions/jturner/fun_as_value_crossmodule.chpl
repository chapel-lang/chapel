module OuterModule {
  use Functions;

  module mod1 {
    proc write_fun( f: func(int, int) ) {
      writeln( f(1) );
    }
  }

  module mod2 {
    use super.mod1;

    proc main {
      proc add1(x: int) {
        return x + 1;
      }
      write_fun(add1);
    }
  }
}
