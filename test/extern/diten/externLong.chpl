require "externLong.h";

use SysCTypes;

extern record hasLong {
  var i: c_long;
  var j: c_longlong;
}

var l: hasLong;

l.i = 1;
l.j = 2;

writeln(l.i);
writeln(l.j);
