use messagefield;
use IO;
use List;

var messageObj = new messageA();
var file = open("out", iomode.r);
var readingChannel = file.reader();

messageObj.deserialize(readingChannel);

writeln(messageObj.a.b == 150);
writeln(messageObj.a.c ==  "String with spaces");
writeln(messageObj.f[0].d == 26);
writeln(messageObj.f[0].e == true);
writeln(messageObj.f[1].d == 36);
writeln(messageObj.f[1].e == false);
writeln(messageObj.g.a == 76);
writeln(messageObj.h[0].a == 26);
writeln(messageObj.h[1].a == 46);
