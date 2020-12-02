use typesTest;
use IO;

var messageObj = new Types();
var file = open("out", iomode.r);
var readingChannel = file.reader();

messageObj.deserialize(readingChannel);

writeln(messageObj.ui64 == 9223372036854);
writeln(messageObj.ui32 == 429496729);
writeln(messageObj.i64 == -600000);
writeln(messageObj.i32 == 214748364);
writeln(messageObj.bo == true);
writeln(messageObj.si64 == -675348989989);
writeln(messageObj.si32 == -214748364);
writeln(messageObj.byt == b"\x97\xB3\xE6\xCC\x01");
writeln(messageObj.st == "Protobuf implementation for chapel");
writeln(messageObj.fi32 == 1000000);
writeln(messageObj.fi64 == 100000000000);
writeln(messageObj.fl == 444.23444);
writeln(messageObj.db == 444444444444.23444);
writeln(messageObj.sf32 == -4567);
writeln(messageObj.sf64 == 6473899292);
