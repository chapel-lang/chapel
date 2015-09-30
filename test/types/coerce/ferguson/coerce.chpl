
record R {
  var x: int;
}

//pragma "coerce"
proc R.coercible(type t) param where t == int return true;

proc R.cast(type t) where t == int {
  return this.x;
}


var rec = new R(10);

var x:int;

writeln(rec);

x = rec;

writeln(x);

// Also check explicit cast.
writeln(rec:int);
