
use IO;

record G {
  var x;
}

record I {
  var x : integral;
}

config type T = G;

proc main() {
  var f = openMemFile();
  {
    f.writer().write(new T(5));
  }
  {
    var val = f.reader().withDeserializer(DefaultDeserializer).read(T(int));
    writeln(val);
  }
}
