
class ConcreteClass {
  var x : int;

  proc chpl__promotionType() type {
    return int;
  }

  proc init(x:int) {
    this.x = x;
    this.complete();
    const foo = 1 + this;
    writeln("ConcreteClass foo = ", foo);
  }

  iter these() {
    for i in 1..10 do yield i*x;
  }
}

class GenericClass {
  type t;
  var x : t;

  proc chpl__promotionType() type {
    return t;
  }

  proc init(type t, val) {
    this.t = t;
    this.x = val;
    this.complete();
    const foo = 1 + this;
    writeln("GenericClass foo = ", foo);
  }

  iter these() {
    for i in 1..10 do yield i*x;
  }
}


proc main() {
  var cc = new borrowed ConcreteClass(2);

  var gc = new borrowed GenericClass(real, 1.0);
}
