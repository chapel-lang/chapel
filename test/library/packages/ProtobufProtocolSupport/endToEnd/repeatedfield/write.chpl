use repeatedfield;
use IO;

var file = open("out", ioMode.cw);
var writingChannel = file.writer(locking=false);

var messageObj = new repeatedField();

messageObj.ui64.pushBack(1);
messageObj.ui64.pushBack(2445464);

messageObj.ui32.pushBack(1);
messageObj.ui32.pushBack(24454);

messageObj.i64.pushBack(-100);
messageObj.i64.pushBack(244540000000);

messageObj.i32.pushBack(-500);
messageObj.i32.pushBack(2445489);

messageObj.bo.pushBack(true);
messageObj.bo.pushBack(false);

messageObj.si64.pushBack(-500);
messageObj.si64.pushBack(-24454890000);

messageObj.si32.pushBack(-50);
messageObj.si32.pushBack(-2445489);

messageObj.fi32.pushBack(67);
messageObj.fi32.pushBack(8907);

messageObj.fi64.pushBack(500);
messageObj.fi64.pushBack(2445489000);

messageObj.fl.pushBack(4.12);
messageObj.fl.pushBack(4500.3);

messageObj.db.pushBack(67.2345);
messageObj.db.pushBack(8907980.5657);

messageObj.sfi32.pushBack(-500);
messageObj.sfi32.pushBack(-244548);

messageObj.sfi64.pushBack(-45);
messageObj.sfi64.pushBack(-4500000000);

messageObj.byt.pushBack(b"\x01\x87\x76");
messageObj.byt.pushBack(b"\x00\x01\x02\x03");

messageObj.st.pushBack("aniket");
messageObj.st.pushBack("String with spaces");

messageObj.serialize(writingChannel);
