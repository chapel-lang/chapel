
use IO;

record R {
  type T;
  var x : T;
}

proc main() {
  var f = openMemFile();
  {
    f.writer().write(new R(int, 5));
  }
  {
    var val = f.reader().read(R);
    writeln(val);
  }
}
