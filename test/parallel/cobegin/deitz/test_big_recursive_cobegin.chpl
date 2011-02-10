config const n: int = 1024;

proc foo(i: int) {
  if i < n {
    _debugWriteln(here.id + " pre " + i);
    cobegin {
      foo(i+1);
      ;
    }
    _debugWriteln(here.id + " post " + i);
  }
}

foo(1);
