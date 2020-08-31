use enums;
use IO;
use List;

var messageObj = new enumTest();
var file = open("out", iomode.r);
var readingChannel = file.reader();

messageObj.deserialize(readingChannel);

writeln(messageObj.a == color.blue);
writeln(messageObj.b == 564);

var lst1 = new list(color);
lst1.append(color.blue);
lst1.append(color.green);
writeln(messageObj.c == lst1);

writeln(messageObj.d == enumTest_fruit.orange);

var lst2 = new list(enumTest_fruit);
lst2.append(enumTest_fruit.orange);
lst2.append(enumTest_fruit.apple);
writeln(messageObj.e == lst2);
