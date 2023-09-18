record A : writeSerializable {
  var x:int;
  proc serialize(writer, ref serializer) throws {
    var loc = writer.readWriteThisFromLocale();
    writeln("in A.serialize loc=", loc.id);
    writer.write(x);
  }
}
record B : writeSerializable {
  var a:A;
  proc serialize(writer, ref serializer) throws {
    var loc = writer.readWriteThisFromLocale();
    writeln("in B.serialize loc=", loc.id);
    writer.write(a);
  }
}

on Locales[1] {

  var r = new B(new A(1));
  writeln(r);
}

