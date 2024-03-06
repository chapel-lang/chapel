
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
<<<<<<< HEAD
    var val = f.reader(locking=false).read(R);
=======
    var val = f.reader().read(R(?));
>>>>>>> e71cc1fc7c (Update some tests that are properly generating errors)
    writeln(val);
  }
}
