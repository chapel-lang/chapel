var n: int;

def foo(i: int) {
  n = n + 1;
  writeln(i);
}

def bar(i: int) return n + i;

foo(bar(1..10));
