record C
{
  var i:int;

  operator =(ref lhs: C, rhs:C)
  {
    writeln("Called =(:C, :C)");
    writeln("Old value was ", lhs.i);
    lhs.i = rhs.i;
    writeln("New value is ", lhs.i);
  }
}

var c = new C(3);
var d = new C(7);

writeln("c = ", c);
writeln("d = ", d);

c = d;

writeln("c = ", c);
writeln("Done.");
