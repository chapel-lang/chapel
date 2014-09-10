var x: int;

proc foo() ref {
  if !setter then
    compilerError("foo() used in value context!");
  return x;
}

foo() = 2;
writeln(x);
