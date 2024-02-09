// The original example in #21292

var D = {-1..1,-1..1};
var A: [D] bool;

A[(0,0)] = true;

forall Exy in D {
  if !A[Exy] then continue;
  var neighbours = 0;

  writeln(+ reduce ([xy in {-1..1,-1..1}] xy));

  writeln(neighbours);
}
