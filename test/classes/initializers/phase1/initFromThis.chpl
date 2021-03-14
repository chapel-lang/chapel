// Based on issue #12695

record R {
  var x : int;

  proc init() {
    ref A = this;
    A.x = 10;
    const ref B = this;
    writeln(B);
    var foo = this;
    writeln(foo);
  }

  proc init=(other:R) {
    ref A = this;
    A.x = other.x;
    const ref B = this;
    writeln(B);
    var foo = this;
    writeln(foo);
  }
}

class C {
  var x : int;

  proc init() {
    ref A = this;
    A.x = 10;
    const ref B = this;
    writeln(B);
    var foo = this;
    writeln(foo);
  }
}

proc main() {
  var X : R;
  var Y = X;
  var Z = new C();
}
