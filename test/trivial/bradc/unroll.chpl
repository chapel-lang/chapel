
def foo(x: int) {
  writeln("Got an int");
}

def foo(x: real) {
  writeln("Got a real");
}

def foo(x: string) {
  writeln("Got a string");
}

def unrollme(x ...?numargs) {
  for param i in 1..numargs {
    foo(x(i));
  }
  writeln();
}

unrollme(1,2,3,4);

unrollme(1, 2.3, "4.5.6", 78);


