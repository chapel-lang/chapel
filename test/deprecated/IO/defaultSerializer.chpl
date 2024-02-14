use IO;

var f = openMemFile();
{
  f.writer(serializer=new DefaultSerializer(), locking=false).write("hello!");
}
{
var s = f.reader(deserializer=new DefaultDeserializer(), locking=false).read(string);
  writeln(s);
}
