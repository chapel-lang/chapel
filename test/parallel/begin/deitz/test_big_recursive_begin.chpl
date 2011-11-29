config const n: int = 4096;

proc foo(i: int) {
  var x: sync int;
  var y: single int;
  if i < n {
    sync {
      begin {_debugWriteln(here.id, " pre ", x); y = x;}
      begin {x = i; foo(i+1); x = i;}
    }
    _debugWriteln(here.id, " post ", y);
  }
}

foo(1);
