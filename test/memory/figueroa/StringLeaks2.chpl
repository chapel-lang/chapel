// This test case shows that there is a memory leak due to the second assignment
// to s.  s should be freed, but only if it was assigned to twice.

use MemDiagnostics, IO;

config const n = 1;

proc foo () {
  var stdout = (new file(1)).writer();
  var s = "Good bye";
  if n > 0 then
    s = "Nos vemos";
  stdout.writeln(s);
}

var m1 = memoryUsed();
foo(); 
var m2 = memoryUsed();

writeln("Amount of leaked memory after calling foo(): ", m2:int - m1:int);
