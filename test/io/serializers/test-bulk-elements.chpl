
use IO;
use CTypes;
use Types;

//
// A basic test to ensure that DefaultRectangular can invoke the optional
// writeBulkElements and readBulkElements methods.
//

record Serializer {
  var def : IO.binarySerializer;

  proc ref serializeValue(writer, const val) {
    if isArrayType(val.type) {
      val.serialize(writer, this);
    } else {
      def.serializeValue(writer.withSerializer(def), val);
    }
  }

  record ArraySerializer {
    var writer;
    proc startDim(size:int) throws {}
    proc endDim() throws {}
    proc endArray() throws {}
    proc writeElement(const element: ?) throws {}

    proc writeBulkElements(data: c_ptr(?eltType), numElements: int) throws
    where isNumericType(eltType) {
      writeln("writeBulkElements");
    }
  }

  proc startArray(writer: fileWriter(?), size: int) {
    return new ArraySerializer(writer);
  }
}

record Deserializer {
  var def : IO.binaryDeserializer;

  proc ref deserializeType(reader, type valType) {
    if isArrayType(valType) {
      return valType.deserializeFrom(reader, this);
    } else {
      return def.deserializeType(reader.withDeserializer(def), valType);
    }
  }

  record ArrayDeserializer {
    var reader;
    proc startDim() throws {}
    proc endDim() throws {}
    proc endArray() throws {}
    proc readElement(type eltType) throws { var x: eltType; return x; }

    proc readBulkElements(data: c_ptr(?eltType), numElements: int) throws
    where isNumericType(eltType) {
      writeln("readBulkElements");
    }
  }

  proc startArray(reader: fileReader(?)) {
    return new ArrayDeserializer(reader);
  }
}

proc main() {
  var A : [1..10] int = 1..10;

  var f = openMemFile();
  f.writer(serializer=new Serializer(), locking=false).write(A);

  var X = f.reader(deserializer=new Deserializer(), locking=false).read(A.type);
}
