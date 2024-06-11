use IO;

record MyRecord : serializable {
  var i: int;
  proc init(i: int = 0) { this.i = i; }
  proc init(reader, ref deserializer) throws {
    this.init();
    deserialize(reader, deserializer);
  }
}

config const fileName = "test.txt";
config const debug = true;

// Open up a file to work with.
// Note that fileName not exist or have no contents
var f = open(fileName, ioMode.cwr);

proc ref MyRecord.deserialize(reader, ref deserializer) throws {
  i = reader.read(int);
  reader.readNewline();
}

proc MyRecord.serialize(writer, ref serializer) throws {
  writer.write(i);
  writer.writeNewline();
}

{
  var reader = f.reader(locking=false);

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
