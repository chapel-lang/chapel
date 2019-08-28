module M {
  class X {
    var x:int;
  }
}

module Y {
  use M;
  proc main() {
    var y = new unmanaged M.X(1);

    writeln('y = ', y);

    delete y;
  }
}
