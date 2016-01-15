
record MyRecord {
  var a: int;
  var b: int;
}


writef("testing json stdout: %jt\n", new MyRecord(1,2));

var f = opentmp();

{
  var writer = f.writer();
  var str = '';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  var got = reader.readf("%jt", r);

  writeln("got is ", got);
  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer();
  var str = ' ';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  var got = reader.readf("%jt", r);

  writeln("got is ", got);
  writeln("Read: ", r);

  reader.close();
}


{
  var writer = f.writer();
  var str = '{"c":3}';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  var err:syserr;

  var got = reader.readf("%~jt", r, error=err);

  writeln("got is ", got, " error is ", errorToString(err));
  writeln("Read: ", r);

  reader.close();
}


{
  var writer = f.writer();
  var str = '{"b":1, "a":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  reader.readf("%jt", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer();
  var str = '{"junk":3,"b":1, "a":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  reader.readf("%~jt", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer();
  var str = '{"b":1, "junk":3, "a":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  reader.readf("%~jt", r);

  writeln("Read: ", r);

  reader.close();
}



{
  var writer = f.writer();
  var str = '{"b":2, "a":1, "junk":3}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  reader.readf("%~jt", r);

  writeln("Read: ", r);

  reader.close();
}


{
  var writer = f.writer();
  var str = '{"b":2, "junk":3, "other junk" : "bla\\"rg", "a":1}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  reader.readf("%~jt", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer();
  var str = '{"b":2, "junk":[],"junk1":[1], "junk2" : [ 2,3 ],"junk3":["a","b","c"], "a":1}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  reader.readf("%~jt", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer();
  var str = '{"b":2, "junk":{}, "junk1" : {"a":-1}, "junk2":{"a":-1,"b":-1},  "a":1}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  reader.readf("%~jt", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer();
  var str = '{"b":1, "record":{"a":-1, "b":-2, "c":[{"a":"-3", "b":"-4"},-5, "d"]}, "a":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var r:MyRecord;

  reader.readf("%~jt", r);

  writeln("Read: ", r);

  reader.close();
}



f.close();
