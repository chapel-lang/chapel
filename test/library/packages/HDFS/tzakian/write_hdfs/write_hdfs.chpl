use RecordParser, HDFS;

// =========== TEST 1 ====================
var hdfs = hdfs_chapel_connect("default", 0);

writeln("=== Opening in write Only mode and writing to HDFS ===\n");

var f = hdfs.hdfs_chapel_open("/tmp/preadv_test1.txt", iomode.cw);
var ff = f.writer();
ff.write("line 1\nline 2\nline 3\nline 4"
          + "\nline 5\nline 6\nline 7\nline 8\nDONE\n");
ff.close();
f.close();

writeln("\n\n=== Finished writing and closed. Now opening for read ===\n");

var fi = hdfs.hdfs_chapel_open("/tmp/preadv_test1.txt", iomode.r);
var fff = fi.reader(); 
var str:  string;
while(fff.readline(str)) {
  write("read line from HDFS: " + str);
}
fff.close();
fi.close();
writeln("===== Finished test one! =========\n");

// ============= TEST 2 ================
writeln("=== Starting test two ===\n");
writeln("=== Opening for write and writing ===\n");

var c = hdfs.hdfs_chapel_open("/tmp/beer_test.txt", iomode.cw);
var ch = c.writer();
ch.write("\nBeer: 123 555 \n\nName: one \nBeer: 456 \n\nName: two \nBeer: 789 \n\nName: three \nBeer: 101112 \n\nName: four");
ch.close();
c.close();

writeln("=== Finished writing and closed. Now opening for reading and parsing into records ===\n");

var r  = hdfs.hdfs_chapel_open("/tmp/beer_test.txt", iomode.r);
var fr = r.reader();
var strreg = "\\s*Beer:\\s*(.*?)\\s*Name:\\s*(.*?)\\s*";

record Bar {
  var Beer: string;
  var Name: string;
}

var M = new RecordReader(Bar, fr, strreg);

writeln("======== test of get() ============");

var a = M.get();
var d = M.get();
writeln(a, " ", d);

writeln("========== test of stream() ==============");
for r in M.stream() do
  writeln(r);

writeln();
fr.close();
r.close();

writeln("== Finished Test two! ===");
// ======= END TEST TWO ======


hdfs.hdfs_chapel_disconnect();
