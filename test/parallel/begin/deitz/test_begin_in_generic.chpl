def bar(type t) {
  writeln("bar");
}

def foo(type t) {
  begin bar(t);
}

foo(int);
