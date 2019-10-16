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
  return borrowed C(t);
}

var myC: getType(int)?;
myC = new (getType(int))();
writeln(myC);
