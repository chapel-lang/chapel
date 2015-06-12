
record R {
  var x: int;
}

/* Should this be x.canCoerceTo(type t) returning a bool?
   Is there any benefit to having x be the run-time type
    (vs user type)?
   What about classes SubC : C, and if I provide C.canCoerceTo(type t)?
   Clearly a SubC should be able to dispatch there... making me
   think that we want to use the normal coercion rules for non-user
   defined coercions.
   */
pragma "coerce"
proc _allow_coerce(type t, x: R) param where t == int return false;

proc _cast(type t, x: R) where t == int {
  return x.x;
}


var rec = new R(10);

var x:int;

writeln(rec);

x = rec;

writeln(x);
