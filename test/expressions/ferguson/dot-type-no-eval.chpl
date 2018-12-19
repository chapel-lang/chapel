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

writeln(a(b(c(1))).type:string);
