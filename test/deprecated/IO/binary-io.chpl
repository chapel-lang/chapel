
use BinaryIO;
import IO.openMemFile;

proc main() {
  var f = openMemFile();
  {
    f.writer(serializer=new BinarySerializer()).write("hello, world");
  }
  {
    var s = f.reader(deserializer=new BinaryDeserializer()).read(string);
    writeln(s);
  }
}
