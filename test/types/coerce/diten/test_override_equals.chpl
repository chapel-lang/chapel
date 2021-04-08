operator +(a: uint(64), b: int(64)) {
  writeln("Found my plus");
  return a + b:uint(64);
}

//
// This no longer works because (a) ChapelBase.chpl also adds an
// overload of ==(int, int) and (b) standard modules do == on
// enums which find these two ==() calls ambiguous (neither is
// closer than the other, as in this test, and we don't supply 
// ==() on enums for simplicity and to avoid the (arguably
// unnecessary) generic instantiation).
//
// If we wanted to support this overload by the user, I think
// the way to do it would be to provide ==() for matching enum
// types.  That was a bigger change than I wanted to make for
// this test, though.  Particularly since I'm not convinced
// that it's a good idea for users to be overloading ==.
//
operator int.==(a: int, b: int) {
  writeln("Found my ==, but it's going to give != instead");
  return a != b;
} 
var m: uint(64) = 1;
var n: int = 2;
var a: int = 1;
writeln(m + n);
writeln(a == n);
