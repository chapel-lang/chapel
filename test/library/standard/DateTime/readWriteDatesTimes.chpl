use Time, IO, JSON;

proc testReadWrite(dt) {
  var dt2: dt.type;
  var mem = openMemFile();
  var writer = mem.writer(serializer = new JsonSerializer());
  writer.writef("%?", dt);
  var reader = mem.reader(deserializer = new JsonDeserializer());

  reader.readf("%?", dt2);

  writeln(dt.type:string, ": ", dt == dt2);
}

var dt = dateTime.now();
testReadWrite(dt);
testReadWrite(dt.getDate());
testReadWrite(dt.getTime());
