use IO;

var prev = read(int);
var bigger = 0;
do {
  var depth: int;
  const success = read(depth);
  if (depth > prev) then
    bigger += 1;
  prev = depth;
} while (success);
writeln(bigger);
