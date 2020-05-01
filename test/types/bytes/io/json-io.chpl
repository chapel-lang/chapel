use IO;

record Rec {
  var foo: int;
  var bar: bytes;
}

var r = new Rec(10, "some bytes":bytes);

writef("testing default stdout: %t\n", r);
writef("testing json stdout: %jt\n", r);

var f = opentmp();
{
  var writer = f.writer();
  var str = '{"foo": 4, "bar":b"some written bytes"}';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r: Rec;

  var got = reader.readf("%jt", r);

  writeln("got is ", got);
  writeln("Read: ", r);

  reader.close();
}
f.close();
