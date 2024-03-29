use ProtobufProtocolSupport.WireEncoding;
use IO;

var f = openMemFile();

// tests for unsignedVarint
unsignedVarintAppend(0, f.writer(locking=false));
writeln(unsignedVarintConsume(f.reader(locking=false))(0) == 0);

unsignedVarintAppend(86942, f.writer(locking=false));
writeln(unsignedVarintConsume(f.reader(locking=false))(0) == 86942);

// tests for uint64
uint64AppendBase(18446744073709551615, f.writer(locking=false));
writeln(uint64ConsumeBase(f.reader(locking=false)) == 18446744073709551615);

uint64AppendBase(1, f.writer(locking=false));
writeln(uint64ConsumeBase(f.reader(locking=false)) == 1);

// tests for uint32
uint32AppendBase(4294967295, f.writer(locking=false));
writeln(uint32ConsumeBase(f.reader(locking=false)) == 4294967295);

uint32AppendBase(1, f.writer(locking=false));
writeln(uint32ConsumeBase(f.reader(locking=false)) == 1);

// tests for int64
int64AppendBase(9223372036854775807, f.writer(locking=false));
writeln(int64ConsumeBase(f.reader(locking=false)) == 9223372036854775807);

int64AppendBase(-9223372036854775807, f.writer(locking=false));
writeln(int64ConsumeBase(f.reader(locking=false)) == -9223372036854775807);

// tests for int32
int32AppendBase(2147483647, f.writer(locking=false));
writeln(int32ConsumeBase(f.reader(locking=false)) == 2147483647);

int32AppendBase(-2147483647, f.writer(locking=false));
writeln(int32ConsumeBase(f.reader(locking=false)) == -2147483647);

// tests for bool
boolAppendBase(true, f.writer(locking=false));
writeln(boolConsumeBase(f.reader(locking=false)) == true);

boolAppendBase(false, f.writer(locking=false));
writeln(boolConsumeBase(f.reader(locking=false)) == false);

// tests for sint32
sint32AppendBase(-2147483647, f.writer(locking=false));
writeln(sint32ConsumeBase(f.reader(locking=false)) == -2147483647);

sint32AppendBase(1, f.writer(locking=false));
writeln(sint32ConsumeBase(f.reader(locking=false)) == 1);

// tests for sint64
sint64AppendBase(-9223372036854775807, f.writer(locking=false));
writeln(sint64ConsumeBase(f.reader(locking=false)) == -9223372036854775807);

sint64AppendBase(2, f.writer(locking=false));
writeln(sint64ConsumeBase(f.reader(locking=false)) == 2);

// tests for bytes
bytesAppendBase(b"testing", f.writer(locking=false));
writeln(bytesConsumeBase(f.reader(locking=false)) == b"testing");

bytesAppendBase(b"\x97\xB3\xE6\xCC\x01", f.writer(locking=false));
writeln(bytesConsumeBase(f.reader(locking=false)) == b"\x97\xB3\xE6\xCC\x01");

// tests for string
stringAppendBase("testing", f.writer(locking=false));
writeln(stringConsumeBase(f.reader(locking=false)) == "testing");

stringAppendBase("String with space", f.writer(locking=false));
writeln(stringConsumeBase(f.reader(locking=false)) == "String with space");

// tests for fixed32
fixed32AppendBase(0, f.writer(locking=false));
writeln(fixed32ConsumeBase(f.reader(locking=false)) == 0);

fixed32AppendBase(2147483647, f.writer(locking=false));
writeln(fixed32ConsumeBase(f.reader(locking=false)) == 2147483647);

// tests for fixed64
fixed64AppendBase(0, f.writer(locking=false));
writeln(fixed64ConsumeBase(f.reader(locking=false)) == 0);

fixed64AppendBase(9223372036854775807, f.writer(locking=false));
writeln(fixed64ConsumeBase(f.reader(locking=false)) == 9223372036854775807);

// tests for float
floatAppendBase(4.6789, f.writer(locking=false));
writeln(floatConsumeBase(f.reader(locking=false)) == 4.6789);

floatAppendBase(4000.34, f.writer(locking=false));
writeln(floatConsumeBase(f.reader(locking=false)) == 4000.34);

// tests for double
doubleAppendBase(4.578694, f.writer(locking=false));
writeln(doubleConsumeBase(f.reader(locking=false)) == 4.578694);

doubleAppendBase(444444444444.23444, f.writer(locking=false));
writeln(doubleConsumeBase(f.reader(locking=false)) == 444444444444.23444);

// tests for sfixed32
sfixed32AppendBase(2, f.writer(locking=false));
writeln(sfixed32ConsumeBase(f.reader(locking=false)) == 2);

sfixed32AppendBase(-2147483647, f.writer(locking=false));
writeln(sfixed32ConsumeBase(f.reader(locking=false)) == -2147483647);

// tests for sfixed64
sfixed64AppendBase(2, f.writer(locking=false));
writeln(sfixed64ConsumeBase(f.reader(locking=false)) == 2);

sfixed64AppendBase(-9223372036854775807, f.writer(locking=false));
writeln(sfixed64ConsumeBase(f.reader(locking=false)) == -9223372036854775807);

f.close();
