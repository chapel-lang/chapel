proc f(out arg) {
  arg = 1;
  writeln("f(out arg: ", arg.type:string, ")");
}

proc main() {
  var x: real;
  f(x);
}
