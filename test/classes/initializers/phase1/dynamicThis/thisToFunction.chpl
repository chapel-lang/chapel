
class Parent {
  var x : int;

  proc init() {
    this.x = 5;
  }

  proc inheritedMethod() {
    writeln("Parent.inheritedMethod()");
  }
}

proc foobar(thing) {
  writeln("thing.type = ", thing.type:string);
  thing.x += 1;
  thing.inheritedMethod();
  writeln();
}

proc multi(a,b) {
  writeln("a.type = ", a.type:string);
  writeln("b.type = ", b.type:string);
  a.inheritedMethod();
  b.inheritedMethod();
  writeln();
}

class Child : Parent {
  var y : real;

  proc init() {
    writeln("----- phase one -----");
    this.y = x ** 2;
    inheritedMethod();
    foobar(this);
    multi(this, this);
    this.complete();
    writeln("----- phase two -----");
    foobar(this);
  }

  override proc inheritedMethod() {
    writeln("Child.inheritedMethod()");
  }
}

var c = new owned Child();
writeln("c = ", c);
