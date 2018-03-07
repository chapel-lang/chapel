
class ConcreteClass {
  var x : int;

  proc chpl__promotionType() type {
    return int;
  }

  proc init(x:int) {
    this.x = x;
    this.initDone();
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
    this.initDone();
    const foo = 1 + this;
    writeln("GenericClass foo = ", foo);
  }

  iter these() {
    for i in 1..10 do yield i*x;
  }
}


proc main() {
  var cc = new ConcreteClass(2);
  delete cc;

  var gc = new GenericClass(real, 1.0);
  delete gc;
}
