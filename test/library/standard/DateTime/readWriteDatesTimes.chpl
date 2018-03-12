use DateTime;

proc testReadWrite(dt) {
  var dt2: dt.type;
  var mem = openmem();
  var writer = mem.writer();
  writer.write(dt:string);
  var reader = mem.reader();

  reader.readf("%jt", dt2);

  writeln(dt.type:string, ": ", dt == dt2);
}

var dt = datetime.now();
testReadWrite(dt);
testReadWrite(dt.getdate());
testReadWrite(dt.gettime());
