use IO;

var complexNumber: complex = 1.2 + 3.4i;
var f = open("freadComplex.txt", ioMode.r).reader(locking=false);
var i: int = 0;
var numTestCases: int = 8;

while (i <= numTestCases) {
  f.read(complexNumber);         
  writeln(complexNumber);
  i += 1;
}	
