use IO;

config const filename = "asserteof.test.nums";

var infile = open(filename, ioMode.cwr);

{
  var writer = infile.writer(locking=false);
  writer.writeln(1);
  writer.writeln(2);
  writer.close();
}

{
  var reader = infile.reader(locking=false);
  var x: string;
  var y: string;
  x = reader.readLine();
  y = reader.readLine();
  reader.assertEOF();
  reader.close();
}

writeln("Past First Check");

{
  var reader = infile.reader(locking=false);
  var x: string;
  x = reader.readLine();
  reader.assertEOF();
  reader.close();
}

infile.close();
