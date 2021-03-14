var A = [2,3,4,5];

var byinout_set_A$: sync bool;
var other_set_A$: sync bool;

proc byinout(inout args ...?k, error:int)
{
  args[0] = 5;
  byinout_set_A$.writeEF(true); 
  var x = other_set_A$.readFE(); // wait for A = 0
  // args[1] should have different storage than A
  writeln(args[0]);
  // now since args is inout, I expect this function
  // to return A = 5
}

proc sametime()
{
  var x = byinout_set_A$.readFE(); // wait for A = 5
  A = 0;
  writeln(A);
  other_set_A$.writeEF(true);
}

proc byref(ref args ...?k)
{
  byinout((...args), error=2);
}

writeln(A);
cobegin with (ref A) {
  byref(A);
  sametime();
}
writeln(A);
