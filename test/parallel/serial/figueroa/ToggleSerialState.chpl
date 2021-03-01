// The purpose of this test is to ensure that it is possible to
// change the serial state from true to false and vice-versa.

use Time;

config const runs = 4;
config const n = 5;

for i in 1..runs {
  var s: sync int = 0,
      in_order = true;
  serial i % 2 > 0 do
    coforall j in 1..n with (ref in_order) {
      sleep((n-j) / 10.0);
      var t = s.readFE() + 1;
      if in_order then
        in_order = t == j;
      s.writeEF(j);
    }
  writeln("Run ", i, " was executed ",
          if in_order then "in" else "out of", " order");
}
