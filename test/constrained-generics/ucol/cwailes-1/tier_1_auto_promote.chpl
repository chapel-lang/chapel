interface I(type T) {
  proc inc(x:T):bool;
}

proc inc(x:int):int {
  return x + 1;
}

proc f(a:?T) : T where implements I(T) {
  return inc(a);
}

implements I([1..3] int);
var a:[1..3] int = (1,2,3);

// Writes [2,3,4]
writeln(f(a));
