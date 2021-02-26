use repeatedfield;
use IO;
use List;

var messageObj = new repeatedField();
var file = open("out", iomode.r);
var readingChannel = file.reader();

messageObj.deserialize(readingChannel);

var lst1 = new list(uint(64));
lst1.append(1);
lst1.append(2445464);
writeln(messageObj.ui64 == lst1);

var lst2 = new list(uint(32));
lst2.append(1);
lst2.append(24454);
writeln(messageObj.ui32 == lst2);

var lst3 = new list(int(64));
lst3.append(-100);
lst3.append(244540000000);
writeln(messageObj.i64 == lst3);

var lst4 = new list(int(32));
lst4.append(-500);
lst4.append(2445489);
writeln(messageObj.i32 == lst4);

var lst5 = new list(bool);
lst5.append(true);
lst5.append(false);
writeln(messageObj.bo == lst5);

var lst6 = new list(int(64));
lst6.append(-500);
lst6.append(-24454890000);
writeln(messageObj.si64 == lst6);

var lst7 = new list(int(32));
lst7.append(-50);
lst7.append(-2445489);
writeln(messageObj.si32 == lst7);

var lst8 = new list(uint(32));
lst8.append(67);
lst8.append(8907);
writeln(messageObj.fi32 == lst8);

var lst9 = new list(uint(64));
lst9.append(500);
lst9.append(2445489000);
writeln(messageObj.fi64 == lst9);

var lst10 = new list(real(32));
lst10.append(4.12);
lst10.append(4500.3);
writeln(messageObj.fl == lst10);

var lst11 = new list(real(64));
lst11.append(67.2345);
lst11.append(8907980.5657);
writeln(messageObj.db == lst11);

var lst12 = new list(int(32));
lst12.append(-500);
lst12.append(-244548);
writeln(messageObj.sfi32 == lst12);

var lst13 = new list(int(64));
lst13.append(-45);
lst13.append(-4500000000);
writeln(messageObj.sfi64 == lst13);

var lst14 = new list(bytes);
lst14.append(b"\x01\x87\x76");
lst14.append(b"\x00\x01\x02\x03");
writeln(messageObj.byt == lst14);

var lst15 = new list(string);
lst15.append("aniket");
lst15.append("String with spaces");
writeln(messageObj.st == lst15);
