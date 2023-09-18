class C {
  type t;
  var x: t;
  proc init(type t) {
    this.t = t;
    init this;
    writeln("In C.init()");
  }
}

proc getType() type {
  return C;
}

var myC = new unmanaged (getType())(int);
writeln(myC);
delete myC;
