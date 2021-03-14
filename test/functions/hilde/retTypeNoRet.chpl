proc foo(x:bool) : int
{
  if x then return 10;
  else writeln("In foo.");
}

writeln(foo(true));

proc bar(x:int) : real
{
  select x {
    when 0 do return 0.0;
    when 1 do writeln(1.0);
    otherwise halt("Invalid argument in bar");
  }
}

writeln(bar(0));
