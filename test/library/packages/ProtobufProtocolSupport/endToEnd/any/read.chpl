use any;
use IO;

var messageObj = new anyTest();
var obj = new test();
var file = open("out", iomode.r);
var readingChannel = file.reader();

messageObj.deserialize(readingChannel);

messageObj.anyfield.unpack(obj);

writeln(messageObj.a == 123);
writeln(obj.a == "chapel");
writeln(obj.b == true);
