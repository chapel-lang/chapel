use IO;

enum MyEnum { Type1, Type2, Type3 };

record MyRecord {
  var a: string;
  var b: MyEnum;
}

var f = opentmp();

{
  var writer = f.writer();
  var r:MyRecord = new MyRecord("Hello", MyEnum.Type3);
  writef("Writting JSON: %jt\n", r);
  writer.writef("%jt", r);
  writer.close();
}

{
  var reader = f.reader();
  var r:MyRecord;
  reader.readf("%jt", r);
  writeln("Read: ", r);
  reader.close();
}

f.close();
