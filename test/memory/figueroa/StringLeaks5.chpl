// In this test case, s2 should be freed after the conditional statement,
// but before the last writeln in foo.

use Memory;

config const n = 1;

proc foo () {
  var m1 = memoryUsed();
  var s1 = "Honey, I'm home!  Where's my dinner?",
      s2 = s1;
  if n < 0 then
    for i in 1..2 do
      for j in 1..2 do
        writeln(s2);
  var m2 = memoryUsed();
  writeln(s1);
  return m2:int - m1:int;
}

writeln("Amount of leaked memory after calling foo(): ", foo());
