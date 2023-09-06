
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

  var f1 = openMemFile();
  {
    f1.writer(serializer=new binarySerializer()).write("hello, world");
  }
  {
    var s = f1.reader(deserializer=new binaryDeserializer()).read(string);
    writeln(s);
  }
}
