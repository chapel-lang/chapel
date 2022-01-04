proc g(out arg) {
  arg = 1;
  writeln("g(out arg: ", arg.type:string, ")");
}

proc f(out arg) {
  g(arg);
  writeln("f(out arg: ", arg.type:string, ")");
}

proc main() {
  var x: real;
  f(x);
}
