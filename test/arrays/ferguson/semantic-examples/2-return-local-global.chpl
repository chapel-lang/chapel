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
proc g(x) {
  x[1] = one;
}
g(f(true));

// Does this print out 0 0 0 or 1 0 0 ?
writeln(A);
