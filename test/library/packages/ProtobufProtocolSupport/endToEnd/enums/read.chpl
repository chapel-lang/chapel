use enums;
use IO;
use List;

var messageObj = new enumTest();
var file = open("out", ioMode.r);
var readingChannel = file.reader();

messageObj.deserialize(readingChannel);

writeln(messageObj.a == color.blue);
writeln(messageObj.b == 564);

var lst1 = new list(color);
lst1.pushBack(color.blue);
lst1.pushBack(color.green);
writeln(messageObj.c == lst1);

writeln(messageObj.d == enumTest_fruit.orange);

var lst2 = new list(enumTest_fruit);
lst2.pushBack(enumTest_fruit.orange);
lst2.pushBack(enumTest_fruit.apple);
writeln(messageObj.e == lst2);
