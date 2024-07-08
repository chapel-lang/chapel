config param doError=false;

record R {
  type t;
  var x: t;
}

var val: R(?);
proc foo() ref: R(?) {
  return val;
}

if !doError then
  val = new R(int);

var a = foo();
writeln(a);
