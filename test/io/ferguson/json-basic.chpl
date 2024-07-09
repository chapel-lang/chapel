
use IO, JSON;

record MyRecord {
  var a: int;
  var b: int;
}

writef("testing default stdout: %?\n", new MyRecord(1,2));
stdout
  .withSerializer(jsonSerializer)
  .writef("testing json stdout: %?\n", new MyRecord(1,2));

var f = openTempFile();

{
  var writer = f.writer(locking=false);
  var str = '';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  var got = reader.readf("%?", r);

  writeln("got is ", got);
  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer(locking=false);
  var str = ' ';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  var got = reader.readf("%?", r);

  writeln("got is ", got);
  writeln("Read: ", r);

  reader.close();
}


{
  var writer = f.writer(locking=false);
  var str = '{"c":3}';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;
  var got: bool;

  try! {
    got = reader.readf("%?", r);
  } catch e: IllegalArgumentError {
    writeln("illegal argument error");
  }
  writeln("got is ", got);
  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer(locking=false);
  var str = '{"a":1, "b":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer(locking=false);
  var str = '{"a" : 1, "b" : 2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}


{
  var writer = f.writer(locking=false);
  var str = '{"b":1, "a":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}


{
  var writer = f.writer(locking=false);
  var str = '{"junk":3,"b":1, "a":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer(locking=false);
  var str = '{"b":1, "junk":3, "a":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}



{
  var writer = f.writer(locking=false);
  var str = '{"b":2, "a":1, "junk":3}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}


{
  var writer = f.writer(locking=false);
  var str = '{"b":2, "junk":3, "other junk" : "bla\\"rg", "a":1}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer(locking=false);
  var str = '{"b":2, "junk":[],"junk1":[1], "junk2" : [ 2,3 ],"junk3":["a","b","c"], "a":1}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer(locking=false);
  var str = '{"b":2, "junk":{}, "junk1" : {"a":-1}, "junk2":{"a":-1,"b":-1},  "a":1}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}

{
  var writer = f.writer(locking=false);
  var str = '{"b":1, "record":{"a":-1, "b":-2, "c":[{"a":"-3", "b":"-4"},-5, "d"]}, "a":2}\n';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new jsonDeserializer(), locking=false);

  var r:MyRecord;

  reader.readf("%?", r);

  writeln("Read: ", r);

  reader.close();
}



f.close();
