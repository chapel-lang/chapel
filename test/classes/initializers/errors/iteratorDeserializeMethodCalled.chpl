use IO;

class IteratorDeserialize {
  iter deserialize(reader: fileReader(?),
                 ref deserializer: reader.deserializerType) throws {
    reader.readLiteral("blah");
  }
}

proc main() {
  var fr = openReader("nonexistent.txt"); // Isn't real, we fail at compilation
  var i = new IteratorDeserialize();
  fr.readln(i);
}
