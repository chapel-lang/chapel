class C {
  type t;
  var x: t;
  def initialize() {
    writeln("side effect ", x);
  }
}

def foo(type t) {
  var x: t;
  writeln(x);
}

foo(C(real));
