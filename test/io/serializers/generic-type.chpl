
use IO;

record R {
  type T;
  var x : T;
}

proc main() {
  var f = openMemFile();
  {
    f.writer(locking=false).write(new R(int, 5));
  }
  {
    var val = f.reader(locking=false).read(R);
    writeln(val);
  }
}
