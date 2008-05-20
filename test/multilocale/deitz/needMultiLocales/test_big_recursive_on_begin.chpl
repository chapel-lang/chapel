config const n: int = 1024;

def foo(i: int) {
  if i < n {
    _debugWriteln(here.id, " pre ", i);
    sync {
      on Locales(i%2) do begin foo(i+1);
    }
    _debugWriteln(here.id, " post ", i);
  }
}

foo(1);
