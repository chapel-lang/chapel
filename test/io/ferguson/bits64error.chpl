// Configuration Constants
use IO;

config const n = 2048;
config const fname = "error.data";

{
  var myfile = open(fname, iomode.cw).writer();
  for i in 0..#n {
    var x:uint = i:uint;
    myfile.writebits(x, 63);
  }
  myfile.close();
}
writeln("Done writing");

{
  // Read data (FILE MUST BE 8 MB)
  var myfile = open(fname, iomode.r).reader(kind=ionative);
  var sum:uint = 0;

  for i in 0..#n
  {
    var x:uint;
    myfile.readbits(x, 63);
    sum += x;
  }

  myfile.close();

  writeln("Done reading, sum = ", sum);
}
