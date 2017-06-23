class BaseClass {
  var a, b: int;

  proc init(b: int) {
    this.b = b;
    super.init();
  }
}

class DerivedClass: BaseClass {
  var c: int;

  proc init(b:int, c:int) {
    this.c = c;
    super.init(b);
  }
}

var D = new DerivedClass(1,2);

writeln(D);

delete D;
