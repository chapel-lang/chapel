// This test checks whether nonnull attributes are generated for
// attributes that have ref intent

//CHECK: i64 @f_chpl(
//CHECK: i64* nonnull
//CHECK: i64* nonnull
//CHECK: i64
//CHECK-NOT: nonnull
//CHECK: )

proc f(ref x : int, const ref y : int, z : int)
{
  x = 100;
  return x+y*z;
}

var a : int = 100;
var b : int = 200;
var c : int = 300;

f(a, b, c);
