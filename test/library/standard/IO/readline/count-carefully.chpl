use IO;

var buf: [1..10] uint(8);
var numRead = 0;
var rc: bool;

rc = stdin.readline(buf, numRead);
writeln("readln returnd ", rc, ", numRead ", numRead, ": ", buf[1..numRead]);

rc = stdin.readline(buf, numRead);
writeln("readln returnd ", rc, ", numRead ", numRead, ": ", buf[1..numRead]);
