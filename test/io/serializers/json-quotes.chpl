
use IO, Json;

proc main() {
  var f = openMemFile();
  var x = "with, \"quotes\" !";
  {
    f.writer().withSerializer(JsonSerializer).writeln(x);
  }
  {
    var got = f.reader().withDeserializer(JsonDeserializer).read(string);
    assert(x == got);
  }
}
