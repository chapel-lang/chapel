// Test a begin block with a reference to a class variable

class C {
  var x: int;
}

var c = new unmanaged C();
c.x = 7;
writeln( c);

begin {
  c.x = 14;
  writeln( c);
  delete c;
}
