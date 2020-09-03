use any;
use IO;

var file = open("out", iomode.cw);
var writingChannel = file.writer();

var messageObj = new anyTest();
var obj = new test();

messageObj.a = 123;

obj.a = "chapel";
obj.b = true;
messageObj.anyfield.pack(obj);

messageObj.serialize(writingChannel);
