use IO;

class IteratorSerialize {
  iter serialize(writer: fileWriter(?),
                 ref serializer: writer.serializerType) throws {
    writer.write("blah");
  }
}
