
use binaryIO;
import IO.openMemFile;

proc main() {
  var f = openMemFile();
  {
    f.writer(serializer=new binarySerializer()).write("hello, world");
  }
  {
    var s = f.reader(deserializer=new binaryDeserializer()).read(string);
    writeln(s);
  }
}
