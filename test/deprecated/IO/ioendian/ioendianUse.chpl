use IO;

var k : ioendian;

var a = ioendian.big;
var b = ioendian.native;
var c = ioendian.little;

var f = openMemFile();
var reader = f.reader(deserializer=new binaryDeserializer(ioendian.big), locking=false);
var writer = f.writer(serializer=new binarySerializer(ioendian.big), locking=false);

var ds = new binaryDeserializer(ioendian.little);
var s = new binarySerializer(ioendian.little);
var r2 = f.reader(deserializer=ds, locking=false);
var w2 = f.writer(serializer=s, locking=false);

writeln(ds.endian);
writeln(s.endian);
