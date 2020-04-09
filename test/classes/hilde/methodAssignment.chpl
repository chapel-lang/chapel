// methodAssignment.cpl
//
// The compiler should call a user-defined assignment method with the 
// signature:
//   proc T.=(rhs:T)
// The implicit "this" argument is assumed to be updated to be a copy of the rhs
// argument.
//

class C
{
  var i:int;

  proc =(rhs:C)
  {
    writeln("Called C.=(:C)");
    writeln("Old value was ", this.i);
    this.i = rhs.i;
    writeln("New value is ", this.i);
  }
}

var c = new C(3);
var d = new C(7);

writeln("c = ", c);
writeln("d = ", d);

c = d;

writeln("c = ", c);
writeln("Done.");
