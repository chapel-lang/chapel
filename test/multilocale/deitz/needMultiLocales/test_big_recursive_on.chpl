config const n: int = 768;

def foo(i: int) {
  if i < n {
    _debugWriteln(here.id, " pre ", i);
    on Locales(i%2) do foo(i+1);
    _debugWriteln(here.id, " post ", i);
  }
}

foo(1);
