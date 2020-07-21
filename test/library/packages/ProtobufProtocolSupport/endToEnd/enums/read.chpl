use enums;
use IO;
use List;

var messageObj = new enumTest();
var file = open("out", iomode.r);
var readingChannel = file.reader();

messageObj.parseFromInputFile(readingChannel);

writeln(messageObj.a == color.blue);
writeln(messageObj.b == 564);

var lst = new list(color);
lst.append(color.blue);
lst.append(color.green);
writeln(messageObj.c == lst);
