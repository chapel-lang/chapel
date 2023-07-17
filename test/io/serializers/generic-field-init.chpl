
use IO;

record G {
  var x;
}

record I {
  var x : integral;
}

config type T = G;
config param useInit : bool = false;

// Test call to a reader-initializer, make sure it doesn't exist.
proc main() {
  var f = openMemFile();
  {
    f.writer().write(new T(5));
  }
  {
    var reader = f.reader().withDeserializer(DefaultDeserializer);
    var val = new T(int, reader);
    writeln(val);
  }
}
