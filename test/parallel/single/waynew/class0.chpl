// Test single field in a class

class D {
  var s: single float;
}

var d: D = D();
var f: float;

begin {
  writeln( "2: got ", d.s);
  writeln( "2: got ", d.s);
}
f = 4.0;
writeln( "1: going to sleep with ", f);
sleep( 3);
writeln( "1: woke up. writing ", f);
d.s = f;

