class C {
  type t;
  var x: t;
  proc init(type t) {
    this.t = t;
    this.complete();
    writeln("In C.init()");
  }
}

proc getType() type {
  return C;
}

var myC = new (getType())(int);
writeln(myC);
delete myC;
