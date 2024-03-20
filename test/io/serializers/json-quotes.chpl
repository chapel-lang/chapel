
use IO, JSON;

proc main() {
  var f = openMemFile();
  var x = "with, \"quotes\" !";
  {
    f.writer(locking=false).withSerializer(jsonSerializer).writeln(x);
  }
  {
    var got = f.reader(locking=false).withDeserializer(jsonDeserializer).read(string);
    assert(x == got);
  }
}
