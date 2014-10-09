// precedence.chpl
//
// Demonstrate relative precedence of user-overrides of
//   the default constructor
// vs.
//   _defaultOf()
//

record R {
  var i:int = 2;
  var r:real = 1.41421;
}

proc _defaultOf(type t) where t == R
{
  var result:R = noinit;
  result.i = 4;
  result.r = 2.0;
  return result;
}

proc R.R()
{
  this.i = 8;
  this.r = 2.82842;
}

var r:R;

writeln(r); // Expect (i = 4, r = 2.0).
