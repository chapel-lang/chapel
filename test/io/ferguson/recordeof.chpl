use IO;

record MyRecord {
  var i: int;
  proc init(i: int = 0) { this.i = i; }
  proc init(f: fileReader) throws {
    this.init();
    this.i = f.readln(int);
  }
}

config const fileName = "test.txt";
config const debug = true;

// Open up a file to work with.
// Note that fileName not exist or have no contents
var f = open(fileName, ioMode.cwr);

proc MyRecord.readThis(r: fileReader) throws {
  i = r.read(int);
  r.readNewline();
}

proc MyRecord.writeThis(w: fileWriter) throws {
  w.write(i);
  w.writeNewline();
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
