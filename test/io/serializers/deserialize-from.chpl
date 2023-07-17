
use IO;

record R {
  var x : int;

  proc serialize(writer: fileWriter, ref serializer) {
    writer.write(x);
  }

  proc type deserializeFrom(reader: fileReader, ref deserializer) {
    return new R(reader.read(int));
  }
}

record G {
  var x;

  proc serialize(writer: fileWriter, ref serializer) {
    writer.write(x);
  }

  proc type deserializeFrom(reader: fileReader, ref deserializer) {
    type fieldType = __primitive("field by num", this, 1);
    return new G(reader.read(fieldType));
  }
}

class C {
  var x : int;

  override proc serialize(writer: fileWriter, ref serializer) {
    writer.writeLiteral("<");
    writer.write(x);
    writer.writeLiteral(">");
  }

  proc type deserializeFrom(reader: fileReader, ref deserializer) {
    type retType = this;
    reader.readLiteral("<");
    var ret = new retType(reader.read(int));
    reader.readLiteral(">");
    return ret;
  }

  proc equals(other: borrowed C) {
    return this.x == other.x;
  }
}

proc test(val, type T) {
  var f = openMemFile();
  {
    f.writer().write(val);
  }
  {
    var read = f.reader().read(T);

    if isClassType(T) {
      if isNilableClassType(T) {
        if val == nil then
          assert(read == nil);
        else assert(val!.equals(read!));
      } else {
        assert(val.equals(read));
      }
    } else {
      assert(read == val);
    }
  }
}

proc main() {
  test(new R(5), R);
  test(new G(42.0), G(real));

  // test 'nil' case to make sure serializers can still control the reading of
  // the 'nil' string (as compared to something like "null" for json).
  test(nil, unmanaged C?);
  test(new C(5), owned C);
  writeln("SUCCESS");
}
