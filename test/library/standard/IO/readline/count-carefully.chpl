use IO;

var buf: [1..10] uint(8);
var numRead = 0;
var rc: bool;

numRead = stdin.readLine(buf);
writeln("readln returnd numRead ", numRead, ": ", buf[1..numRead]);

numRead = stdin.readLine(buf);
writeln("readln returnd numRead ", numRead, ": ", buf[1..numRead]);
