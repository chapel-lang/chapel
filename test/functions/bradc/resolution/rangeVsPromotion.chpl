def f(x: int, y: int) {
  writeln("In f(:int, :int)");
}

def f(x: range, y) {
  writeln("In f(:range, :any)");
}

f(1..3, 2);
