
class Parent {
  var a : int;
  var b : real;

  proc init() {
    writeln("Parent.init()");
    this.a = 1;
    this.b = 5.0;
  }

  proc init(a, b) {
    writeln("Parent.init(a, b)");
    this.a = a;
    this.b = b;
  }

  proc parentOnly() {
    writeln("Parent.parentOnly()");
  }

  proc inheritedMethod() {
    writeln("Parent.inheritedMethod()");
  }
}

class Child : Parent {
  var x : int;
  var y : real;

  proc init() {
    writeln("phase one");

    this.x = a * 5;
    this.y = b * -1.0;
    parentOnly();
    inheritedMethod();

    this.complete();

    writeln("phase two");
    inheritedMethod();
  }

  proc init(a, b, x, y) {
    super.init(a, b);
    writeln("phase one");

    this.x = x;
    this.y = y;
    parentOnly();
    inheritedMethod();

    this.complete();

    writeln("phase two");
    inheritedMethod();
  }

  proc init(q) {
    writeln("Child this.init()");
    this.init(q, q, q, q);
    writeln("phase two");
    inheritedMethod();
  }

  override proc inheritedMethod() {
    writeln("Child.inheritedMethod()");
  }
}

proc main() {
  writeln("----- implicit super.init -----");
  var implicit = new unmanaged Child();
  writeln("implicit = ", implicit);
  delete implicit;

  writeln();

  writeln("----- explicit super.init -----");
  var explicit = new unmanaged Child(1, 10.0, 100, 1000.0);
  writeln("explicit = ", explicit);
  delete explicit;

  writeln();

  writeln("----- this.init -----");
  var thisInit = new unmanaged Child(1);
  writeln("thisInit = ", thisInit);
  delete thisInit;
}
