
use IO;
use FormatHelper;
use Map, List;

record R {
  var x : int;
  var y : real;
}

proc test(val) {
  printDebugFmt(val);
  var f = openTempFile();
  {
    var w = f.writer(serializer=FormatWriter, locking=false);
    w.writef("%?\n", val);
  }
  {
    var r = f.reader(deserializer=FormatReader, locking=false);
    var x : val.type;
    r.readf("%?\n", x);

    if val == x then writeln("SUCCESS");
    else writeln("FAILURE");
  }
}

proc main() {
  var val = new R(5, 42.0);
  test(val);

  var m : map(string, int);
  m.add("A", 1);
  m.add("B", 2);
  m.add("C", 3);
  test(m);

  var li : list(int) = [1, 2, 3, 4, 5];
  test(li);
}
