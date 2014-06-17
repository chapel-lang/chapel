// Code which is unreachable in a generic or parameter-controlled
// function should be removed to avoid unreachable code warnings.

proc foo(param gnarly : bool = false, reeg: int)
{
  if ! gnarly then return reeg % 5;

  // when gnarly is false, this code should not be reachable
  // and the compiler error should not fire.
  // compilerError("How did we get here?");
  return 0;
}

proc main()
{
  // Should output "2".
  writeln(foo(reeg = 7));
}

