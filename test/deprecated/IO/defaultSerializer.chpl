use IO;

var f = openMemFile();
{
  f.writer(serializer=new DefaultSerializer()).write("hello!");
}
{
var s = f.reader(deserializer=new DefaultDeserializer()).read(string);
  writeln(s);
}
