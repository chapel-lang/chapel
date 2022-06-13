use IO;

record MyRecord {
  var i: int;
}

config const fileName = "test.txt";
config const debug = true;

// Open up a file to work with.
// Note that fileName not exist or have no contents
var f = open(fileName, iomode.cwr);

proc MyRecord.readThis(f) throws {
  readWriteHelper(f);
}

proc MyRecord.writeThis(f) throws {
  readWriteHelper(f);
}

proc MyRecord.readWriteHelper(f) throws {
  var nl = new ioLiteral("\n");
  if f.writing then
    f.write(i);
  else
    f.readIt(i);

  if f.writing then f.write(nl); else f.read(nl);
}

{
  // create a reader but specify that we'd like to use single-quoted strings.
  // 0x27 is ascii for '
  var reader = f.reader(style=new iostyleInternal(string_format=iostringformat.basic:uint(8), string_start = 0x27, string_end = 0x27));

  var rec:MyRecord;
  var i = 1;

  // read until we reach EOF
  // this test should reach EOF before reading any records
  while( reader.read(rec) ) {
    writeln("read ", rec);
  }

  writeln("Done");

  reader.close();
}


