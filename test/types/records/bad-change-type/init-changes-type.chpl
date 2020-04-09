record R {
  type t;
  var field: t;
}

proc R.init(type t) {
  this.t = real;
  field = 1.0;
}

var x: R(int);
writeln(x.type:string, " ", x);
