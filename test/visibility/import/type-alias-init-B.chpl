module C {
  module A {
    record R {
      var field:int;
    }
  }

  module B {
    proc acceptsType(type t) {
      var x = new t();
      return x;
    }
  }

  proc main() {
    var x = B.acceptsType(A.R);
    writeln(x, ": ", x.type : string);
  }
}
