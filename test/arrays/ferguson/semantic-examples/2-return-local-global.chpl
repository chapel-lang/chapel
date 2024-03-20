use samples;
// one defined in samples as new R(1)

var A:[1..3] R;

proc f(x:bool) {
  if x then return A;
  else {
    var tmp:[1..2] R;
    return tmp;
  }
}
proc g(ref x) {
  x[1] = one;
}
var t = f(true);
g(t);

// Does this print out 0 0 0 or 1 0 0 ?
writeln(A);
