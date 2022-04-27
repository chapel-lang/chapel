use IO;

var prev = read(int);
var bigger = 0;
var depth: int;
while read(depth) {
  if depth > prev then
    bigger += 1;
  prev = depth;
}
writeln(bigger);
