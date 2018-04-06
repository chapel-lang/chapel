class C {
  type t;
  var x: t;
  proc init(type t) {
    this.t = t;
    this.complete();
    writeln("In C.init()");
  }
}

proc getType(type t) type {
  return C(t);
}

var myC = new (getType(int))(int);
writeln(myC);
delete myC;
