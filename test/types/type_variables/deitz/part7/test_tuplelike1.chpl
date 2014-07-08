record TupleType {
  type t;
  var f : t;
}

record AnonTuple {
  var f;
}

proc =(ref a:TupleType, b:AnonTuple)
{ a.f = b.f; }

var x = new AnonTuple(f = 1);
var y : TupleType(int);

writeln(x);
writeln(y);

y = x;

writeln(y);
