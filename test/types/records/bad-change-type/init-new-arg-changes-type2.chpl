class C {
  type t;
  var field: t;
}

proc C.init(type t) {
  this.t = t;
  field = 1.0;
}

type t = C(int);
var y = new t(t=real);
writeln(y.type:string, " ", y);
