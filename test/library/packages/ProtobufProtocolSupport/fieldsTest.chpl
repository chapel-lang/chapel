use ProtobufProtocolSupport.Fields;
use ProtobufProtocolSupport.WireEncoding;
use IO;

var f = openmem();

uint64Append(92233720368, 1, f.writer(kind=iokind.little, locking=false));
var r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, varint)); // Check for fieldNumber and wireType
writeln(uint64Consume(r) == 92233720368); // Check for value

uint32Append(4294967294, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, varint)); // Check for fieldNumber and wireType
writeln(uint32Consume(r) == 4294967294); // Check for value

int64Append(92233720368547758, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, varint)); // Check for fieldNumber and wireType
writeln(int64Consume(r) == 92233720368547758); // Check for value

int32Append(2147483646, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, varint)); // Check for fieldNumber and wireType
writeln(int32Consume(r) == 2147483646); // Check for value

boolAppend(true, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, varint)); // Check for fieldNumber and wireType
writeln(boolConsume(r) == true); // Check for value

sint32Append(-214748364, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, varint)); // Check for fieldNumber and wireType
writeln(sint32Consume(r) == -214748364); // Check for value

sint64Append(-9223372036854775, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, varint)); // Check for fieldNumber and wireType
writeln(sint64Consume(r) == -9223372036854775); // Check for value

bytesAppend(b"\x97\xB3\xE6\xCC\x01", 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(bytesConsume(r) == b"\x97\xB3\xE6\xCC\x01"); // Check for value

stringAppend("String with space", 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, lengthDelimited)); // Check for fieldNumber and wireType
writeln(stringConsume(r) == "String with space"); // Check for value

fixed32Append(300000, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, fixed32Type)); // Check for fieldNumber and wireType
writeln(fixed32Consume(r) == 300000); // Check for value

fixed64Append(400000000000, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, fixed64Type)); // Check for fieldNumber and wireType
writeln(fixed64Consume(r) == 400000000000); // Check for value

floatAppend(4000.34, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, fixed32Type)); // Check for fieldNumber and wireType
writeln(floatConsume(r) == 4000.34); // Check for value

doubleAppend(444444444444.23444, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, fixed64Type)); // Check for fieldNumber and wireType
writeln(doubleConsume(r) == 444444444444.23444); // Check for value

sfixed32Append(-4000, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, fixed32Type)); // Check for fieldNumber and wireType
writeln(sfixed32Consume(r) == -4000); // Check for value

sfixed64Append(-400000000, 1, f.writer(kind=iokind.little, locking=false));
r = f.reader(kind=iokind.little, locking=false);
writeln(tagConsume(r) == (1, fixed64Type)); // Check for fieldNumber and wireType
writeln(sfixed64Consume(r) == -400000000); // Check for value

f.close();
