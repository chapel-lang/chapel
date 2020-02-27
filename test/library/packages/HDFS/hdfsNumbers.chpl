/*
   unit test writes lots of uint(8)s to an hdfs file, then reads from hdfs.
   during the read, this test will break if the values are not what is
   expected.
*/
use HDFS only;
use IO;
use SysCTypes;

config const path = "/tmp/lots-of-numbers.txt";

config const n = 10000;
config const bufsz = 0;

extern var qbytes_iobuf_size:size_t;

proc main() {

  if bufsz > 0 {
    qbytes_iobuf_size = bufsz:size_t;
  }

  var fs = HDFS.connect();

  {
    var wfd = fs.open(path, iomode.cw);
    var w = wfd.writer(iokind.native);

    for i in 0..#n {
      w.write(i);
    }

    w.close();
    wfd.close();
  }

  {
    var rfd = fs.open(path, iomode.r);
    var r = rfd.reader(iokind.native);

    for i in 0..#n {
      var offset = r.offset();
      var j:int;
      r.read(j);
      if j != i {
        writeln("Error : at offset ", offset, " expected ", i, " but got ", j);
      }
    }

    r.close();
    rfd.close();
  }
}
