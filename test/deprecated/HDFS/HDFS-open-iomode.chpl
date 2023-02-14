use HDFS only;
use IO;

config const path = "/tmp/testfile.txt";
var fs = HDFS.connect();

var msg = "This is a test\n";

var f = fs.open(path, ioMode.cw);
var w = f.writer();
w.write(msg);

w.close();
f.close();

