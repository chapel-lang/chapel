class C {
  type t;
  var field: t;
}

proc C.init(type t) {
  this.t = real;
  field = 1.0;
}

type t = C(int);
var y = new t();
writeln(y.type:string, " ", y);

record R {
  type t;
  var field: t;
}

proc R.init(type t) {
  this.t = real;
  field = 1.0;
}

type tt = R(int);
var yy = new tt();
writeln(yy.type:string, " ", yy);
