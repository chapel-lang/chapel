var a:int(32) = 1;
var count:int(64) = 2;
for i in a..#count { // should result in compilation error
  writeln(i);
}
writeln(a..#count); // compilation error here if not above
