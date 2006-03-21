var complexNumber: complex = 1.2 + 3.4i;
var f: file = file(filename = "freadComplex.txt", mode = "r");
var i: int = 0;
var numTestCases: int = 8;

f.open;

while (i <= numTestCases) {
  fread(f, complexNumber);         
  writeln(complexNumber);
}	
