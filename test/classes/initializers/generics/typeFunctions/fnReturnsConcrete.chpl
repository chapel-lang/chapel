class C {
  type t;
  var x: t;
  proc init(type t) {
    this.t = t;
    init this;
    writeln("In C.init()");
  }
}

proc getType(type t) type {
  return unmanaged C(t);
}

var myC = new (getType(int))();
writeln(myC);
delete myC;
