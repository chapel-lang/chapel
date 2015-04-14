
record R {
  var x: int;
}

pragma "coerce"
proc _allow_coerce(type t, x: R) where t == int return true;

proc _cast(type t, x: R) where t == int {
  return x.x;
}


var rec = new R(10);

var x:int;

writeln(rec);

x = rec;

writeln(x);
