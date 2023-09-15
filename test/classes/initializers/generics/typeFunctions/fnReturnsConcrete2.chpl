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
  return owned C(t);
}

var myC: getType(int)?;
myC = new (getType(int))();
writeln(myC);
