interface I(T) {
  proc foo(T):T;
}

implements I(int);

proc foo(x:?U):U {
  return x;
}

proc foo(x:int):int {
  return x + 1;
}

proc bar(x:?T):T where implements I(T) {
  return foo(x);
}

// Writes 42
writeln(bar(41));
