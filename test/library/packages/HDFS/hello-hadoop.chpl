use HDFS only;
use IO;

config const path = "/tmp/testfile.txt";

proc main() {

  var fs = HDFS.connect();

  var msg = "This is a test\n";
  {
    var f = fs.open(path, ioMode.cw);
    var w = f.writer();
    w.write(msg);
    w.close();
    f.close();
  }

  {
    var f = fs.open(path, ioMode.r);
    var r = f.reader();
    var s:string;
    r.readLine(s);
    assert(s == msg);
    f.close();
  }

  writeln("Test file matches, OK");
}
