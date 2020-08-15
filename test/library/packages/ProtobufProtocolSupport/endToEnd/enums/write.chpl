use enums;
use IO;

var file = open("out", iomode.cw);
var writingChannel = file.writer();

var messageObj = new enumTest();

messageObj.a = color.blue;

messageObj.b = 564;

messageObj.c.append(color.blue);
messageObj.c.append(color.green);

messageObj.d = enumTest_fruit.orange;

messageObj.e.append(enumTest_fruit.orange);
messageObj.e.append(enumTest_fruit.apple);

messageObj.serialize(writingChannel);
