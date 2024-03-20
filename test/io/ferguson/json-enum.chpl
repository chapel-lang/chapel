use IO, JSON;

enum MyEnum { Type1, Type2, Type3 };

record MyRecord {
  var a: string;
  var b: MyEnum;
}

var f = openTempFile();

var jsonOut = stdout.withSerializer(jsonSerializer);

{
  var writer = f.writer(serializer = new jsonSerializer(), locking=false);
  var r:MyRecord = new MyRecord("Hello", MyEnum.Type3);
  jsonOut.writef("Writing JSON: %?\n", r);
  writer.writef("%?", r);
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
