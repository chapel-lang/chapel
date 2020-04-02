record R {
  type t;
  var field: t;
}

proc R.init(type t) {
  this.t = t;
  field = 1.0;
}

type tt = R(int);
var yy = new tt(t=real);
writeln(yy.type:string, " ", yy);
