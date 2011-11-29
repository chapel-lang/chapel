proc f(x: int, y: int) {
  writeln("In f(:int, :int)");
}

proc f(x: range, y) {
  writeln("In f(:range, :any)");
}

f(1..3, 2);
