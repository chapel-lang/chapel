record TupleType {
  type t;
  var f : t;
}

proc TupleType.init=(const ref other: TupleType) {
  this.t = other.t;
  this.f = other.f;
}
proc =(ref lhs: TupleType, const ref rhs: TupleType) {
  lhs.f = rhs.f;
}

record AnonTuple {
  var f;
}

proc =(ref a:TupleType, b:AnonTuple) {
  a.f = b.f;
}
proc TupleType.init=(from: AnonTuple) {
  this.t = from.f.type;
  this.f = from.f;
  this.complete();
}
proc _cast(type t: TupleType, from: AnonTuple) {
  var tmp: t = from;
  return tmp;
}

var x = new AnonTuple(f = 1);
var y : TupleType(int);

writeln(x);
writeln(y);

y = x;

writeln(y);
