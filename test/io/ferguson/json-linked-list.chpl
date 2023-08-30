use LinkedLists, IO, JSON;

{
  var mylist = new LinkedList(int);

  mylist.append(1);
  mylist.append(2);
  mylist.append(3);

  stdout.withSerializer(JsonSerializer).writef("testing json write: %?\n", mylist);

  mylist.destroy();
}

var f = openTempFile();
{
  var writer = f.writer();
  var str = '[]';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new JsonDeserializer());

  var mylist:LinkedList(int);

  reader.readf("%?", mylist);

  writeln("Read: ", mylist);

  reader.close();

  mylist.destroy();
}

{
  var writer = f.writer();
  var str    = '[ ]';

  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new JsonDeserializer());

  var mylist:LinkedList(int);

  reader.readf("%?", mylist);

  writeln("Read: ", mylist);

  reader.close();

  mylist.destroy();
}


{
  var writer = f.writer();
  var str    = '[1]';

  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new JsonDeserializer());

  var mylist:LinkedList(int);

  reader.readf("%?", mylist);

  writeln("Read: ", mylist);

  reader.close();

  mylist.destroy();
}

{
  var writer = f.writer();
  var str    = '[1,2]';

  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new JsonDeserializer());

  var mylist:LinkedList(int);

  reader.readf("%?", mylist);

  writeln("Read: ", mylist);

  reader.close();

  mylist.destroy();
}

{
  var writer = f.writer();
  var str    = '[ 1, 2, 3 ]';

  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader(deserializer = new JsonDeserializer());

  var mylist:LinkedList(int);

  reader.readf("%?", mylist);

  writeln("Read: ", mylist);

  reader.close();

  mylist.destroy();
}
