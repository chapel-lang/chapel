use IO, JSON;

var f = openMemFile();
{
  f.writer(serializer=new JsonSerializer(), locking=false).write("hello!");
}
{
var s = f.reader(deserializer=new JsonDeserializer(), locking=false).read(string);
  writeln(s);
}
