use IO, LinkedLists;

record MyRecord {
  var numbers:LinkedList(int); // could it be [1..0] int ?

  proc deinit() {
    numbers.destroy();
  }
}

var myEntry:MyRecord;
myEntry.numbers.push_back(1);
myEntry.numbers.push_back(2);
myEntry.numbers.push_back(3);

writef("testing json write: %jt\n", myEntry);

var f = opentmp();
{
  var writer = f.writer();
  var str = '{"numbers": []}';
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
  var str = '{"numbers": [1]}';
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
  var str = '{"numbers": [1,2]}';
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
  var str = '{"numbers": [1,2,3]}';
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
  var str = '{"numbers": [1,2,3,4]}';
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
  var str = '{"numbers": [1,2,3,4,5]}';
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
  var str = '{"numbers": [1,2,3,4,5,6]}';
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
  var str = '{"numbers": [1,2,3,4,5,6,7]}';
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
  var str = '{"numbers": [1,2,3,4,5,6,7,8]}';
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
  var str = '{"numbers": [1,2,3,4,5,6,7,8,9]}';
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

