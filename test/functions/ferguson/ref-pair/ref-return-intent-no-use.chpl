var x = 0;

proc return_ref() ref
{
  writeln("setter");
  return x;
}
proc return_ref()
{
  writeln("getter");
  return x;
}

return_ref();
