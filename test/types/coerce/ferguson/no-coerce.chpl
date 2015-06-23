
record R {
  var x: int;
}

proc R.coercible(type t, x: R) param where t == int return false;

proc R.cast(type t) where t == int {
  return this.x;
}


var rec = new R(10);

var x:int;

writeln(rec);

x = rec;

writeln(x);
