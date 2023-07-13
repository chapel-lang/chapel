use IO, JSON;

record Rec {
  var foo: int;
  var bar: bytes;
}

var r = new Rec(10, "some bytes":bytes);

writef("testing default stdout: %?\n", r);
stdout.withSerializer(JsonSerializer).writef("testing json stdout: %?\n", r);

var f = openTempFile();
{
  var writer = f.writer();
  var str = '{"foo": 4, "bar":b"some written bytes"}';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new JsonDeserializer());

  var r: Rec;

  var got = reader.readf("%?", r);

  writeln("got is ", got);
  writeln("Read: ", r);

  reader.close();
}
f.close();
