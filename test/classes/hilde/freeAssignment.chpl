// freeAssignment.cpl
//
// The compiler should call user-defined assignment with the preferred
// signature:
//   proc =(ref lhs:T, rhs:T)
// Note in particular the application of ref intent to the LHS argument.
//
// This one tests the definition of assignment as a "free" function.
// There is a parallel test to check that the compiler can also resolve to
// assignment when it is declared as a method (methodAssignment.chpl). 
//

class C
{
  var i:int;
}

proc =(ref lhs:C, rhs:C)
// Works as expected if this signature is used instead:
// proc =(lhs:C, rhs:C)
{
  writeln("Called =(:C, :C)");
  writeln("Old value was ", lhs.i);
  lhs.i = rhs.i;
  writeln("New value is ", lhs.i);
}

var c = new borrowed C(3);
var d = new borrowed C(7);

writeln("c = ", c);
writeln("d = ", d);

c = d;

writeln("c = ", c);
writeln("Done.");
