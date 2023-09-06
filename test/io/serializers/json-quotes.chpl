
use IO, JSON;

proc main() {
  var f = openMemFile();
  var x = "with, \"quotes\" !";
  {
    f.writer().withSerializer(jsonSerializer).writeln(x);
  }
  {
    var got = f.reader().withDeserializer(jsonDeserializer).read(string);
    assert(x == got);
  }
}
