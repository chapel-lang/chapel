proc foo(i:int) : int { return i; }

proc bar() : int checked {
  return foo(1);
}
