use IO;
use oneofs;

var messageObj = new Foo();
var file = open("out", iomode.r);
var readingChannel = file.reader();

messageObj.deserialize(readingChannel);

writeln(messageObj.co == color.red);
writeln(messageObj.name == b"chapel");

writeln(messageObj.mfield.a == 0);
writeln(messageObj.ifield == 45);
