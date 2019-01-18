record R {
  var x:int;
}

proc foo(x:int) : R
{
  return new R(x);
}

var globalReal : real;

proc foo(x:int) ref : real
{
  return globalReal;
}


var x:R = foo(10);
writeln(x);
foo(10) = 1.0;
writeln(globalReal);
