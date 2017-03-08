require "externLong.h";

extern record hasLong {
  var i: c_long;
}

var l: hasLong;

l.i = 1;

writeln(l.i);
