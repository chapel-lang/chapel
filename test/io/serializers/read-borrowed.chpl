
use IO;
use FormatHelper;

class C {
  var x : int;
}

proc main() {
  var c = new C(5);
  var f = openMemFile();
  f.writer(serializer=FormatWriter).write(c);
  var x = f.reader(deserializer=FormatReader).read(borrowed C);
  writeln(x);
}
