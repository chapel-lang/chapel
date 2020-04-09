
use IO;

config var testfile = "test.dat";
var sz = 16777232;
var n = sz / 8;

// create the file.
{
  var tmp = open(testfile, iomode.cwr);

  var och = tmp.writer(kind=iobig);
  for i in 0..#n {
    var x:uint(64) = i:uint(64);
    och.write(x);
  }

  och.close();
  tmp.close();
}

// read the file, check that we get exactly n back.
{
  var tmp = open(testfile, iomode.r);

  var ich = tmp.reader(kind=iobig);
  for i in 0..#n {
    var x:uint(64);
    var got = ich.read(x);
    assert( got );
  }

  var j = 0;
  while true {
    var x:uint(64);
    var got = ich.read(x);
    if !got then break;
    j += 1;
  }

  ich.close();
  tmp.close();

  writeln("Read ", j,  " extra");
  assert( j == 0 );
}

unlink(testfile);

