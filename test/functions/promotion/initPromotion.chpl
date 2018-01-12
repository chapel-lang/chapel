record Foo {
  type eltType;

  proc init(type t) {
    this.eltType = t;
    super.init();
  }

  proc chpl__promotionType() type {
    return eltType;
  }

  iter these() {
    for i in 1..10 {
      yield i:eltType;
    }
  }
}

proc main() {
  var i = new Foo(int);
  var x = i + 1;
  writeln("x = ", x);

  var r = new Foo(real);
  var y = r / 10.0;
  writeln("y = ", y);
}
