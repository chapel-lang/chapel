var x: int;

proc foo() ref {
  return x;
}
proc foo() {
  compilerError("foo() used in value context!");
  return x;
}

foo() = 2;
writeln(x);
