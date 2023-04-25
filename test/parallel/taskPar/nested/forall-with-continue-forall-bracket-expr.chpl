var D = {-1..1,-1..1};
var D2 = D;
var D3 = {-1..1,-1..1};
var A: [D] bool;

A[(0,0)] = true;

//  writeln(+ reduce ([x in {-1..1}] x)); // works here

// The errors in this loop are sensitive to the continue being taken,
// and do not occur if the statement in the loop is chosen by a config
// param

var neighbours = 0;
forall Exy in D {
  if !A[Exy] then continue;

  // writeln(forall xy in {-1..1,-1..1} do xy); // nil
  // writeln(forall xy in {-1..1} do xy); // nil
  writeln([xy in {-1..1,-1..1}] xy); // nil

  // writeln(forall xy in D do xy); // works
  // writeln(forall xy in D2 do xy); // works
  // writeln(forall xy in D3 do xy); // works

  //writeln(+ reduce ([x in {-1..1}] x)); // nil

  //  writeln(+ reduce ([xy in D] xy)); // works

  //writeln(+ reduce ([xy in {-1..1}] xy)); // nil

  // writeln(+ reduce ([xy in {-1..1,-1..1}] xy)); // nil, or child from flat

  //  neighbours = + reduce ([xy in {-1..1,-1..1}] A[Exy+xy]); // nil

  // forall xy in {-1..1,-1..1} do writeln(A[Exy+xy]); // works

  // neighbours = + reduce ([xy in D] A[Exy+xy]);      // works

  //  writeln(+ reduce ({-1..1,-1..1})); // works

}
writeln(neighbours);
