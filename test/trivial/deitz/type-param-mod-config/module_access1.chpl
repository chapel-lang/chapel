module M {
  var x: int;
  var y: int;
}

module N {
  var x: int;
  var y: int;
}

module O {
  use M only;
  use N only;
  proc foo() {
    M.x = 1;
    M.y = 2;
    N.x = 3;
    N.y = 4;
  }
}

module P {
  use M only;
  use N only;
  proc bar() {
    writeln(M.x, M.y, N.x, N.y);
  }
}

module Q {
  use O;
  use P;
  class M {
    var x: int;
  }
  proc main() {
    bar();
    foo();
    bar();

    var N = new shared M(5);

    writeln(N.x);
  }
}
