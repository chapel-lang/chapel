use IO;

class IteratorDeserialize {
  iter deserialize(reader: fileReader(?),
                 ref deserializer: reader.deserializerType) throws {
    reader.readLiteral("blah");
  }
}
