/*
   hdfsRead100.chpl

   unit test writes 100 uint(8)s to an hdfs file, then reads from hdfs.
   during the read, this test will break if the values are not what is 
   expected.
*/
use IO;
use Curl;
use HDFS;

const config url:string;

proc main() {
   assert(url.startsWith("hdfs://"), "url does not start with an hdfs path");

   var wfd = open(fn, iomode.cw);
   var w = fd.writer(iokind.native);

   for i in 0..#100 { w.write(i:uint(8)); }

   w.close(); wfd.close(); 

   var rfd = open(fn, iomode.r);
   var r = fd.reader(iokind.native);

   for i in 0..#100 { var j:uint(8); w.read(j); assert(j == i); }

   r.close(); rfd.close();
}

