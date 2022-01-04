// Test single field in a class with multiple readers.

use Time;

class D {
  var s: single real;
}

var d: unmanaged D = new unmanaged D();

// Do not delete d until every begin has executed
sync {
  var i: int;

  begin writeln( "got ", d.s.readFF());
  begin writeln( "got ", d.s.readFF());
  begin writeln( "got ", d.s.readFF());
  begin writeln( "got ", d.s.readFF());

  i = 4;
  writeln( "1: going to sleep with ", i);

  sleep( 3);

  writeln( "1: woke up. writing ", i);
  d.s.writeEF(i);

  sleep( 1);

  begin writeln( "got ", d.s.readFF());

  sleep( 1);

  begin writeln( "got ", d.s.readFF());
}

delete d;
