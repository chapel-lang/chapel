// Test single field in a class with multiple readers.

use Time;

class D {
  var s: single real;
}

var d: D = new D();
var i: int;

begin writeln( "got ", d.s);
begin writeln( "got ", d.s);
begin writeln( "got ", d.s);
begin writeln( "got ", d.s);

i = 4;
writeln( "1: going to sleep with ", i);
sleep( 3);
writeln( "1: woke up. writing ", i);
d.s = i;

sleep( 1);
begin writeln( "got ", d.s);
sleep( 1);
begin writeln( "got ", d.s);
