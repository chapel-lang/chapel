record Foo {
  type eltType;

  proc init(type t) {
    this.eltType = t;
    super.init();

    const vals = this + 1;
    for i in vals do writeln(i);
  }

  proc chpl__promotionType() type {
    return eltType;
  }

  iter these() {
    for i in 1..10 do yield i:eltType;
  }
}

proc main() {
  var f = new Foo(int);
}
