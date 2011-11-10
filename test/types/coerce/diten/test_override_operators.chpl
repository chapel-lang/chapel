proc +(a: uint(64), b: int(64)) {
  writeln("Found my plus");
  return a + b:uint(64);
}

//
// This part of this test could arguably be considered unfair since
// changing the definition of integer equality could break all sorts
// of internal things.  Better would be to define it to be the same
// as usual but with a writeln; or to define it for a pair of int
// types that don't normally support equality, as with the + above.
// I'm not going to make either change for now, though, since this
// is still working...  Just noting it since Michael pointed it out
// during this code review.
//
proc ==(a: int, b: int) {
  writeln("Found my ==, but it's going to give != instead");
  return a != b;
} 
var m: uint(64) = 1;
var n: int = 2;
var a: int = 1;
writeln(m + n);
writeln(a == n);
