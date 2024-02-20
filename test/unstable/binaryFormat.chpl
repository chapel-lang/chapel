
use IO;

use ObjectSerialization;

class C {
  var x : int;
}

proc main() {
  var f = openMemFile();
  {
    writeln("----- WRITING -----");
    var w = f.writer(serializer=new binarySerializer(), locking=false);
    var c = new owned C(5);
    w.write(c);
    w.write("abcdefg");
  }
  {
    writeln("----- READ BY VALUE -----");
    var r = f.reader(deserializer=new binaryDeserializer(), locking=false);
    var c = new owned C();
    r.read(c);

    var s : string;
    try {
      r.read(s);
    } catch e {
      writeln("string error: ", e.message());
    }

    writeln(c);
    writeln(s);
  }
  {
    writeln("----- READ TYPE -----");
    var r = f.reader(deserializer=new binaryDeserializer(), locking=false);
    var c = r.read(owned C);
    writeln(c);
    try {
      var s = r.read(string);
      writeln(s);
    } catch e {
      writeln("string error: ", e.message());
    }
  }
}
