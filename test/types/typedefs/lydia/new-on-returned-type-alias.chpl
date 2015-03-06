record R {
  var x: int;
}

proc foo() type {
  return R;
}

type t = foo();
var myR = new t();
writeln(myR);
