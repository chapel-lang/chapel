use Functions;

module mod1 {
  def write_fun( f: func(int, int) ) {
    writeln( f(1) );
  }
}

module mod2 {
  use mod1;

  def main {
    def add1(x: int) {
      return x + 1;
    }
    write_fun(add1);
  }
}

