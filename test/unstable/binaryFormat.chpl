
use IO;

class C {
  var x : int;
}

proc main() {
  var f = openMemFile();
  {
    writeln("----- WRITING -----");
    var w = f.writer(serializer=new binarySerializer());
    var c = new owned C(5);
    w.write(c);
    w.write("abcdefg");
  }
  {
    writeln("----- READ BY VALUE -----");
    var r = f.reader(deserializer=new binaryDeserializer());
    var c = new owned C();
    r.read(c);
    var s : string;
    r.read(s);

    writeln(c);
    writeln(s);
  }
  {
    writeln("----- READ TYPE -----");
    var r = f.reader(deserializer=new binaryDeserializer());
    var c = r.read(owned C);
    var s = r.read(string);
    writeln(c);
    writeln(s);
  }
}
