use IO, RecordParser, HDFS;

writeln("============= Start of HDFS->record parser ==========================");

var hdfs = hdfs_chapel_connect("default", 0);
var ff = hdfs.hdfs_chapel_open("/tmp/beer.txt", iomode.r);
var ffr = ff.reader();

record Beer {
  var name:  string;
  var beerId: int;
  var brewerId: int;
  var ABV: real;
  var style: string;
  var appearance: real;
  var aroma: real;
  var palate: real;
  var taste: real;
  var overall: real;
  var time:  int;
  var profileName:  string;
  var text: string;
}

var strt = "\\s*beer/name: (.*)\\s*beer/beerId: (.*)\\s*beer/brewerId: (.*)\\s*beer/ABV: (.*)\\s*beer/style: (.*)\\s*review/appearance: (.*)\\s*review/aroma: (.*)\\s*review/palate: (.*)\\s*review/taste: (.*)\\s*review/overall: (.*)\\s*review/time: (.*)\\s*review/profileName: (.*)\\s*review/text: (.*)";

var N = new RecordReader(Beer, ffr, strt);
writeln("========== test of stream() ==============");
for r in N.stream() do
write(".");
writeln();
ffr.close();
ff.close();

writeln();
writeln();
write("========== Starting basic HDFS/Channels interleaved reading test ==============");
writeln();


var fi = open("data/foo.txt", iomode.r);
// Were already connected..
var f = hdfs.hdfs_chapel_open("/tmp/foo.txt", iomode.r);

var ffi  = f.reader();
var fff = fi.reader(); 

var str:  string;
var str2: string;

while(ffi.readline(str)) {
  fff.readline(str2);
  write("read string from HDFS: " + str);
  write("read string from  LFS: " + str2);
}
ffi.close();
fff.close();

f.close();
fi.close();

writeln("=== Reading from HDFS into data/bar.txt ===\n");

 //This file differes from beer.txt in that its block size is
 //set to 1024 -- and therefore it must have blocks residing on different locales
var r = hdfs.hdfs_chapel_open("/tmp/uly.txt", iomode.r);
var rr = r.reader(); 

var w = open("data/bar.txt", iomode.cw);
var ww = w.writer();

//var str:  string;
while(rr.readline(str)) {
  ww.write(str);
}
rr.close();
r.close();

ww.close();
w.close();
writeln("===== Finished read into data/bar.txt =========");


hdfs.hdfs_chapel_disconnect();
