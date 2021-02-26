var otherGlobal: R;

proc returnOtherGlobal() : R {
  return otherGlobal;
}

var globalR: R = returnOtherGlobal();
writeln("globalR is ", globalR);

record TestRecord {
  var field: R = globalR;
}

class C {
  var x: int;
}

record R {
  var x: int;
  var c: owned C;

  proc init() {
    this.x = 1;
    this.c = new owned C(this.x);
  }

  proc init=(other:R) {
    writeln("init=");
    var test = new TestRecord();
    this.x = other.x;
    this.c = new owned C(this.x);
  }
}
