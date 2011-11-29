use Time;
var year, month, day: int;
(year, month, day) = getCurrentDate();
if year > 2102 then
  writeln("I don't believe that you are still working on Chapel!");
else
  writeln("Chapel is a new parallel programming language.");
