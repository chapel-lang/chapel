use enums;
use IO;

var file = open("out", ioMode.cw);
var writingChannel = file.writer();

var messageObj = new enumTest();

messageObj.a = color.blue;

messageObj.b = 564;

messageObj.c.pushBack(color.blue);
messageObj.c.pushBack(color.green);

messageObj.d = enumTest_fruit.orange;

messageObj.e.pushBack(enumTest_fruit.orange);
messageObj.e.pushBack(enumTest_fruit.apple);

messageObj.serialize(writingChannel);
