use IO;

var k : ioendian;

var a = ioendian.big;
var b = ioendian.native;
var c = ioendian.little;

var f = openMemFile();
var reader = f.reader(deserializer=new binaryDeserializer(ioendian.big));
var writer = f.writer(serializer=new binarySerializer(ioendian.big));

var ds = new binaryDeserializer(ioendian.little);
var s = new binarySerializer(ioendian.little);
var r2 = f.reader(deserializer=ds);
var w2 = f.writer(serializer=s);

writeln(ds.endian);
writeln(s.endian);
