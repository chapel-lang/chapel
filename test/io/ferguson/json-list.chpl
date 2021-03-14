use LinkedLists, IO;

{
  var mylist = new LinkedList(int);

  mylist.append(1);
  mylist.append(2);
  mylist.append(3);

  writef("testing json write: %jt\n", mylist);

  mylist.destroy();
}

var f = opentmp();
{
  var writer = f.writer();
  var str = '[]';
  writeln("Writing JSON: ", str);
  writer.write(str);
  writer.close();
}

{
  var reader = f.reader();

  var mylist:LinkedList(int);

  reader.readf("%jt", mylist);

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
  var reader = f.reader();

  var mylist:LinkedList(int);

  reader.readf("%jt", mylist);

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
  var reader = f.reader();

  var mylist:LinkedList(int);

  reader.readf("%jt", mylist);

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
  var reader = f.reader();

  var mylist:LinkedList(int);

  reader.readf("%jt", mylist);

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
  var reader = f.reader();

  var mylist:LinkedList(int);

  reader.readf("%jt", mylist);

  writeln("Read: ", mylist);

  reader.close();

  mylist.destroy();
}

