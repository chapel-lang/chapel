var globalReal : real;

proc foo(x:int) const ref : real
{
  return 0.0;
}


iter foo(x:int) ref : real
{
  yield globalReal;
}

for a in foo(1) {
  writeln(a);
}
