module M {
  class X {
    var x:int;
  }
}

module Y {
  proc main() {
    var y = new unmanaged M.X(1);

    writeln('y = ', y);

    delete y;
  }
}
