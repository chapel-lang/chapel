var A = [1,3,4,5];

proc byinout(inout args ...?k, error:int)
{
  args[0] = 5;
}

proc byref(ref args ...?k)
{
  byinout((...args), error=2);
}

writeln(A);
byref(A);
writeln(A);
