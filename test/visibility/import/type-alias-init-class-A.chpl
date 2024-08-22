module A {
  class C {
    var field:int;
  }
}

module B {
  proc acceptsType(type t) {
    var x = new t();
    return x;
  }
}

module C {
  import A;
  import B;

  proc main() {
    var x = B.acceptsType(owned A.C);
    writeln(x, ": ", x.type : string);
  }
}
