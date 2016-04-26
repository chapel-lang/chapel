var g = 0;
proc refReturn() ref
{
  if setter then
    return g;
  else
    return 1;
}

writeln(refReturn());
refReturn() = 2;
writeln(g);

