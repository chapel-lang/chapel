
class C : writeSerializable {
  var A : [1..10] sync int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(A.readXX());
  }
}

var c = new unmanaged C();
writeln(c);
delete c;
