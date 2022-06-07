use IO;

var A: [1..20 by 2] uint(8);
var numread: int;

numread = readLine(A);
writeln("Got ", numread, ":", A[1..2*numread by 2]);
numread = readLine(A);
writeln("Got ", numread, ":", A[1..2*numread by 2]);

// read past EOL
numread = readLine(A, maxSize=1);
while (numread > 0) {
  if A[1] == '\n'.toByte() then
    break;
  numread = readLine(A, maxSize=1);
}

numread = stdin.readLine(A);
writeln("Got ", numread, ":", A[1..2*numread by 2]);
numread = stdin.readLine(A);
writeln("Got ", numread, ":", A[1..2*numread by 2]);
