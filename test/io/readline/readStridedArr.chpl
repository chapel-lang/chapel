use IO;

var A: [1..20 by 2] uint(8);
var numread: int;

readline(A, numread);
writeln("Got ", numread, ":", A[1..2*numread by 2]);
readline(A, numread);
writeln("Got ", numread, ":", A[1..2*numread by 2]);

// read past EOL
while (readline(A, numread, amount=1)) {
  if A[1] == '\n'.toByte() then
    break;
}

stdin.readline(A, numread);
writeln("Got ", numread, ":", A[1..2*numread by 2]);
stdin.readline(A, numread);
writeln("Got ", numread, ":", A[1..2*numread by 2]);
