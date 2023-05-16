use ProtobufProtocolSupport.RepeatedFields;
use ProtobufProtocolSupport.WireEncoding;
use IO;
use List;

var f = openMemFile();

var lst1 = new list(uint(64));
lst1.pushBack(1);
lst1.pushBack(2445464);
uint64RepeatedAppend(lst1, 1, f.writer(kind=iokind.little, locking=false));
var r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(uint64RepeatedConsume(r) == lst1); // Check for value

var lst2 = new list(uint(32));
lst2.pushBack(1);
lst2.pushBack(24454);
uint32RepeatedAppend(lst2, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(uint32RepeatedConsume(r) == lst2); // Check for value

var lst3 = new list(int(64));
lst3.pushBack(-100);
lst3.pushBack(244540000000);
int64RepeatedAppend(lst3, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(int64RepeatedConsume(r) == lst3); // Check for value

var lst4 = new list(int(32));
lst4.pushBack(-500);
lst4.pushBack(2445489);
int32RepeatedAppend(lst4, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(int32RepeatedConsume(r) == lst4); // Check for value

var lst5 = new list(bool);
lst5.pushBack(true);
lst5.pushBack(false);
boolRepeatedAppend(lst5, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(boolRepeatedConsume(r) == lst5); // Check for value

var lst6 = new list(int(32));
lst6.pushBack(-50);
lst6.pushBack(-2445489);
sint32RepeatedAppend(lst6, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(sint32RepeatedConsume(r) == lst6); // Check for value

var lst7 = new list(int(64));
lst7.pushBack(-500);
lst7.pushBack(-24454890000);
sint64RepeatedAppend(lst7, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(sint64RepeatedConsume(r) == lst7); // Check for value

var lst8 = new list(bytes);
lst8.pushBack(b"\x01\x87\x76");
lst8.pushBack(b"\x00\x01\x02\x03");
bytesRepeatedAppend(lst8, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
lst8.clear();
lst8.pushBack(b"\x01\x87\x76");
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(bytesRepeatedConsume(r) == lst8); // Check for first value
lst8.clear();
lst8.pushBack(b"\x00\x01\x02\x03");
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(bytesRepeatedConsume(r) == lst8); // Check for second value

var lst9 = new list(string);
lst9.pushBack("aniket");
lst9.pushBack("String with spaces");
stringRepeatedAppend(lst9, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
lst9.clear();
lst9.pushBack("aniket");
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(stringRepeatedConsume(r) == lst9); // Check for first value
lst9.clear();
lst9.pushBack("String with spaces");
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(stringRepeatedConsume(r) == lst9); // Check for second value

var lst10 = new list(uint(32));
lst10.pushBack(67);
lst10.pushBack(8907);
fixed32RepeatedAppend(lst10, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(fixed32RepeatedConsume(r) == lst10); // Check for value

var lst11 = new list(uint(64));
lst11.pushBack(500);
lst11.pushBack(2445489000);
fixed64RepeatedAppend(lst11, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(fixed64RepeatedConsume(r) == lst11); // Check for value

var lst12 = new list(real(32));
lst12.pushBack(4.12);
lst12.pushBack(4500.3);
floatRepeatedAppend(lst12, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(floatRepeatedConsume(r) == lst12); // Check for value

var lst13 = new list(real(64));
lst13.pushBack(67.2345);
lst13.pushBack(8907980.5657);
doubleRepeatedAppend(lst13, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(doubleRepeatedConsume(r) == lst13); // Check for value

var lst14 = new list(int(32));
lst14.pushBack(-500);
lst14.pushBack(-244548);
sfixed32RepeatedAppend(lst14, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(sfixed32RepeatedConsume(r) == lst14); // Check for value

var lst15 = new list(int(64));
lst15.pushBack(-45);
lst15.pushBack(-4500000000);
sfixed64RepeatedAppend(lst15, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(sfixed64RepeatedConsume(r) == lst15); // Check for value

f.close();
