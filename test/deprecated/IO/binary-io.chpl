
use BinaryIO;
import IO.openMemFile;

proc main() {
  var f = openMemFile();
  {
    f.writer(serializer=new BinarySerializer(), locking=false).write("hello, world");
  }
  {
    var s = f.reader(deserializer=new BinaryDeserializer(), locking=false).read(string);
    writeln(s);
  }

  var f1 = openMemFile();
  {
    f1.writer(serializer=new binarySerializer(), locking=false).write("hello, world");
  }
  {
    var s = f1.reader(deserializer=new binaryDeserializer(), locking=false).read(string);
    writeln(s);
  }
}
