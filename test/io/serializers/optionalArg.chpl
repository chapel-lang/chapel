
use IO;
use Json;

record R {
  var x : int;
  var y : real;
}

proc main() {
  var r = new R(5, 42.0);

  writeln("Default: ", r);
  write("JSON: ");
  writeln(r, serializer=new JsonSerializer());

  var f = openMemFile();
  {
    f.writer().write(r, serializer=new JsonSerializer());
  }
  {
    var x = f.reader().read(R, deserializer=new JsonDeserializer());
    writeln("got: ", x);
  }
}
