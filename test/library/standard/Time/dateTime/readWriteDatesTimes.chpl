use Time, IO, JSON;

proc testReadWrite(dt) {
  var dt2: dt.type;
  // var mem = openMemFile();
  var mem = open("readwdt.tmp", ioMode.cwr);
  mem.writer(serializer = new jsonSerializer(), locking=false).write(dt);
  mem.reader(deserializer = new jsonDeserializer(), locking=false).read(dt2);

  writeln(dt.type:string, ": ", dt == dt2);
}

var dt = dateTime.now();
testReadWrite(dt);
testReadWrite(dt.getDate());
testReadWrite(dt.getTime());
