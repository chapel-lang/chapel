def f(x: int) where false {
  writeln("ignored where clause in f()");
}

def g(x: int) where undefined_fn_call(x) {
  writeln("made it into g() too");
}
f(3);
g(4);
