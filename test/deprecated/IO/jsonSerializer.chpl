use IO, JSON;

var f = openMemFile();
{
  f.writer(serializer=new JsonSerializer()).write("hello!");
}
{
var s = f.reader(deserializer=new JsonDeserializer()).read(string);
  writeln(s);
}
