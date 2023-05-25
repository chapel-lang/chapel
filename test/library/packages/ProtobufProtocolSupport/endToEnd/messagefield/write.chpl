use messagefield;
use IO;

var file = open("out", ioMode.cw);
var writingChannel = file.writer();

var messageObj = new messageA();

messageObj.a.b = 150;
messageObj.a.c = "String with spaces";

var tmpObj1:messageC;
var tmpObj2:messageC;
tmpObj1.d = 26;
tmpObj1.e = true;
messageObj.f.pushBack(tmpObj1);
tmpObj2.d = 36;
tmpObj2.e = false;
messageObj.f.pushBack(tmpObj2);

messageObj.g.a = 76;

var tmpObj3:messageA_messageD;
var tmpObj4:messageA_messageD;
tmpObj3.a = 26;
messageObj.h.pushBack(tmpObj3);
tmpObj4.a = 46;
messageObj.h.pushBack(tmpObj4);

messageObj.serialize(writingChannel);
