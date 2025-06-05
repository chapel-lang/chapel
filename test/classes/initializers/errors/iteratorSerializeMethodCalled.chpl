use IO;

class IteratorSerialize {
  iter serialize(writer: fileWriter(?),
                 ref serializer: writer.serializerType) throws {
    writer.write("blah");
  }
}

proc main() {
  var i = new IteratorSerialize();
  writeln(i);
}
