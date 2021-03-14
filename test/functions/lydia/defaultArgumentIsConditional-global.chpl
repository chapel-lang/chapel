var global = true;

proc someFunc(arg2 = if global then 1 else 2) {
  writeln(arg2);
}

proc main() {
  someFunc();
}
