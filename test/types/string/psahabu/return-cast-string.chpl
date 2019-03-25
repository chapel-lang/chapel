
config const cond : bool = true;

proc foo() {
  return "A";
}

proc main() {
  var y = if cond then foo():string else "B";
  writeln(y);
}
