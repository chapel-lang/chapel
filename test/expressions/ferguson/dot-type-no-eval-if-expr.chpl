
config const setting = true;

proc f() {
  writeln("Nothing to see here");
  return 5;
}

writeln(f().type:string);

proc a(arg) {
  writeln("a");
  return 1.0;
}

proc b(arg) {
  writeln("b");
  return "two";
}

proc c(arg) {
  writeln("c");
  return 3;
}

writeln(if setting then a(b(c(1))).type:string else c(b(a(1))).type:string);
writeln(if !setting then a(b(c(1))).type:string else c(b(a(1))).type:string);
