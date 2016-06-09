var A:[1..3] int;

proc f(x:bool) {
  if x then return A;
  else {
    var tmp:[1..0] int;
    return tmp;
  }
}
proc g(x) {
  x[1] = 1;
}
g(f(true));
writeln(A);
