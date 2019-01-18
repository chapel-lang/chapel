var globalArray : [1..4] int;

proc getter(ref arg) ref {
  writeln("REF");
  return arg;
}

proc getter(const ref arg) const ref {
  writeln("CONST REF");
  return arg;
}

proc f(i:int, x) : int {
  if i<=0 then
    return 0;
  return f(i-1, x);
}

proc g(i:int, x) : int {
  if i<=0 then
    return 0;
  return h(i-1, x);
}
proc h(i:int, x) : int {
  return g(i-1, x);
}

proc fset(i:int, x) : int {
  if i==3 then
    x[1] = 1;
  if i<=0 then
    return 0;
  return fset(i-1, x);
}

proc gset(i:int, x) : int {
  if i<=0 then
    return 0;
  return hset(i-1, x);
}
proc hset(i:int, x) : int {
  if i==3 then
    x[1] = 1;
  return g(i-1, x);
}


writeln("recursion: ");
f(3, getter(globalArray));

writeln("mutual recursion: ");
g(3, getter(globalArray));

writeln("set in recursion: ");
fset(3, getter(globalArray));

writeln("set in mutual recursion: ");
gset(3, getter(globalArray));
