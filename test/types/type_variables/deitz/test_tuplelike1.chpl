record TupleType {
  type t;
  var f : t;
}

record AnonTuple {
  var f;
}

var x : AnonTuple(f = 1);
var y : TupleType(integer);

writeln(x);
writeln(y);

y = x;

writeln(y);
