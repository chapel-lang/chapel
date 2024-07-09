
use IO;

record JustTwoRecord : writeSerializable, readDeserializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc ref deserialize(reader, ref deserializer) throws {
    this.x = reader.read(int);
  }

  proc init(x = 0) {
    this.x = x;
  }

  proc init(reader, ref deserializer) {
    init this;
    this.x = reader.read(int);
  }
}

record SerializableRecord : serializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc ref deserialize(reader, ref deserializer) throws {
    this.x = reader.read(int);
  }

  proc init(x = 0) {
    this.x = x;
  }

  proc init(reader, ref deserializer) {
    init this;
    this.x = reader.read(int);
  }
}

record DefaultRecord {
  var x : int;
}

record SerializeMissingRecord {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }
}

record SerializePresentRecord : writeSerializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }
}

record DeserializeMissingRecord : writeSerializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc ref deserialize(reader, ref deserializer) throws {
    this.x = reader.read(int);
  }
}

record DeserializePresentRecord : writeSerializable, readDeserializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc ref deserialize(reader, ref deserializer) throws {
    this.x = reader.read(int);
  }
}

record InitMissingRecord : writeSerializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc init(x = 0) { this.x = x; }

  proc init(reader, ref deserializer) throws {
    init this;
    this.x = reader.read(int);
  }
}

record InitPresentRecord : writeSerializable, initDeserializable {
  var x : int;

  proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc init(x = 0) { this.x = x; }

  proc init(reader, ref deserializer) throws {
    init this;
    this.x = reader.read(int);
  }
}

class JustTwoClass : writeSerializable, readDeserializable {
  var x : int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  override proc deserialize(reader, ref deserializer) throws {
    this.x = reader.read(int);
  }

  proc init(x = 0) {
    this.x = x;
  }

  proc init(reader, ref deserializer) {
    init this;
    this.x = reader.read(int);
  }
}

class SerializableClass : serializable {
  var x : int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  override proc deserialize(reader, ref deserializer) throws {
    this.x = reader.read(int);
  }

  proc init(x = 0) {
    this.x = x;
  }

  proc init(reader, ref deserializer) {
    init this;
    this.x = reader.read(int);
  }
}

class DefaultClass {
  var x : int;
}

class SerializeMissingClass {
  var x : int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }
}

class SerializePresentClass : writeSerializable {
  var x : int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }
}

class DeserializeMissingClass : writeSerializable {
  var x : int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  override proc deserialize(reader, ref deserializer) throws {
    this.x = reader.read(int);
  }
}

class DeserializePresentClass : writeSerializable, readDeserializable {
  var x : int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  override proc deserialize(reader, ref deserializer) throws {
    this.x = reader.read(int);
  }
}

class InitMissingClass : writeSerializable {
  var x : int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc init(x = 0) { this.x = x; }

  proc init(reader, ref deserializer) throws {
    init this;
    this.x = reader.read(int);
  }
}

class InitPresentClass : writeSerializable, initDeserializable {
  var x : int;

  override proc serialize(writer, ref serializer) throws {
    writer.write(x);
  }

  proc init(x = 0) { this.x = x; }

  proc init(reader, ref deserializer) throws {
    init this;
    this.x = reader.read(int);
  }
}

enum mode { no, readVal, readInit }
use mode;

proc test(type T, param read = mode.readVal) {
  writeln("----- ", T:string, " -----");
  var f = openMemFile();
  var val = new T(10);
  f.writer(locking=false).write(val);

  if read == readVal {
    var newVal = new T();
    f.reader(locking=false).read(newVal);
    writeln("got: ", newVal);
    const check = if isClassType(T) then newVal! else newVal;
    assert(check.x == 10);
  } else if read == readInit {
    var newVal : T = f.reader(locking=false).read(T);
    writeln("got: ", newVal);
    assert(newVal.x == 10);
  }
}

proc main() {
  test(JustTwoRecord);
  test(DefaultRecord);
  test(SerializableRecord);
  test(SerializeMissingRecord,read=no);
  test(SerializePresentRecord,read=no);
  test(DeserializeMissingRecord);
  test(DeserializePresentRecord);
  test(InitMissingRecord, read=readInit);
  test(InitPresentRecord, read=readInit);

  test(owned JustTwoClass);
  test(owned DefaultClass);
  test(owned SerializableClass);
  test(owned SerializeMissingClass,read=no);
  test(owned SerializePresentClass,read=no);
  test(owned DeserializeMissingClass);
  test(owned DeserializePresentClass);
  test(owned InitMissingClass, read=readInit);
  test(owned InitPresentClass, read=readInit);
}
