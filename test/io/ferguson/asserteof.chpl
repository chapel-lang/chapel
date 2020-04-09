use IO;

config const filename = "asserteof.test.nums";

var infile = open(filename, iomode.cwr);

{
  var writer = infile.writer();
  writer.writeln(1);
  writer.writeln(2);
  writer.close();
}

{
  var reader = infile.reader();
  var x:int;
  var y:int;
  reader.read(x);
  reader.read(y);
  reader.assertEOF();
  reader.close();
}

writeln("Past First Check");

{
  var reader = infile.reader();
  var x:int;
  reader.read(x);
  reader.assertEOF();
  reader.close();
}

infile.close();

