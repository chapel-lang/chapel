module M {
  var x: int;
  var y: int;
}

module N {
  var x: int;
  var y: int;
}

module O {
  def foo() {
    M.x = 1;
    M.y = 2;
    N.x = 3;
    N.y = 4;
  }
}

module P {
  def bar() {
    writeln(M.x, M.y, N.x, N.y);
  }
}

module Q {
  use O;
  use P;
  class M {
    var x: int;
  }
  def main() {
    bar();
    foo();
    bar();
    var N = new M(5);
    writeln(N.x);
  }
}
