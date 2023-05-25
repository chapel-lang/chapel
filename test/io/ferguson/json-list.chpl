use List, IO;

{
  var mylist = new list(int);

  mylist.pushBack(1);
  mylist.pushBack(2);
  mylist.pushBack(3);

  writef("testing json write: %jt\n", mylist);
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
  var reader = f.reader();

  var mylist:list(int);

  reader.readf("%jt", mylist);

  writeln("Read: ", mylist);

  reader.close();
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

  var mylist:list(int);

  reader.readf("%jt", mylist);

  writeln("Read: ", mylist);

  reader.close();
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

  var mylist:list(int);

  reader.readf("%jt", mylist);

  writeln("Read: ", mylist);

  reader.close();
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

  var mylist:list(int);

  reader.readf("%jt", mylist);

  writeln("Read: ", mylist);

  reader.close();
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

  var mylist:list(int);

  reader.readf("%jt", mylist);

  writeln("Read: ", mylist);

  reader.close();
}
