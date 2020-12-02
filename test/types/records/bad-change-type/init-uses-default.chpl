record R {
  type t;
  var field: t;
}

proc R.init(type t=real) {
  this.t = t;
  field = 1.0;
}

var xx: R;
writeln(xx.type:string, " ", xx);
