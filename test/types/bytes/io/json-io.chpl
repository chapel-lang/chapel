use IO, JSON;

record Rec {
  var foo: int;
  var bar: bytes;
}

var r = new Rec(10, "some bytes":bytes);

writef("testing default stdout: %?\n", r);
stdout.withSerializer(jsonSerializer).writef("testing json stdout: %?\n", r);

var f = openTempFile();
{
  var writer = f.writer(locking=false);
  var str = '{"foo": 4, "bar":"some written bytes"}';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r: Rec;

  try {
    var got = reader.readf("%?", r);
    writeln("got is ", got);
    writeln("Read: ", r);
  } catch e : BadFormatError {
    writeln("readf failed");
  } catch e {
    writeln(e);
  }

  reader.close();
}
f.close();
