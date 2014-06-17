record foo
{
  var v:int;
}

proc +(a:foo,b:foo):foo
{
  return new foo(a.v+b.v);
}

var A:[1..10] foo = [i in {1..10}] new foo(i);
var x:foo = + reduce A;
writeln("A=\n",A);
writeln("x=",x);
