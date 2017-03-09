use BlockDist;

var Space = {1..10};
var D = Space dmapped Block(Space);
var A:[D] int;

proc f(const ref x)
{
  writeln(x.locale.id);
}

for i in D {
  f(A[i]);
}

for i in D {
  writeln(A[i].locale.id);
}


