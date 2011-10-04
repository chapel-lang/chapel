proc foo(i:int) : int { return i; }

proc bar(type T, x:T) : T where T == int checked {
  return foo(1);
}
