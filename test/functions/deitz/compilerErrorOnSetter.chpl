var x: int;

proc foo() var {
  if !setter then
    compilerError("foo() used in value context!");
}

foo() = 2;
writeln(x);
