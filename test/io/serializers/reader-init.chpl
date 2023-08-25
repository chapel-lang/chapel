
use IO;
use JSON;

record R {
  var x : int;
  var y : real;

  proc init(x: int = 0, y: real = 0.0) {
    this.x = x;
    this.y = y;
  }

  proc init(reader: fileReader, ref deserializer) {
    var des = deserializer.startRecord(reader, "R");
    this.x = des.readField("x", int);
    this.y = des.readField("y", real);
    des.endRecord();
  }

  proc equals(other: R) {
    return this.x == other.x &&
           this.y == other.y;
  }
}

record G {
  type A;
  type B;
  var x : A;
  var y : B;

  proc init(type A, type B, x: A = 0, y: B = 0) {
    this.A = A;
    this.B = B;
    this.x = x;
    this.y = y;
  }

  proc init(type A, type B, reader: fileReader, ref deserializer) {
    this.A = A;
    this.B = B;
    var des = deserializer.startRecord(reader, "G");
    this.x = des.readField("x", A);
    this.y = des.readField("y", B);
    des.endRecord();
  }

  proc equals(other: G) {
    return this.x == other.x &&
           this.y == other.y;
  }
}

class Parent {
  var x : int;

  proc init(x: int = 0) {
    this.x = x;
  }
  proc init(reader: fileReader, ref deserializer) {
    var des = deserializer.startClass(reader, "Parent");
    this.x = des.readField("x", int);
    des.endClass();
  }

  override proc serialize(writer: fileWriter, ref serializer) {
    var ser = serializer.startClass(writer, "Parent", 1);
    ser.writeField("x", x);
    ser.endClass();
  }

  proc equals(other: borrowed Parent) {
    return this.x == other.x;
  }
}

class Child : Parent {
  var y : real;

  proc init(x: int = 0, y: real = 0.0) {
    super.init(x);
    this.y = y;
  }
  proc init(reader: fileReader, ref deserializer) {
    var des = deserializer.startClass(reader, "Child");
    super.init(reader, des);
    this.y = des.readField("y", real);
    des.endClass();
  }

  override proc serialize(writer: fileWriter, ref serializer) {
    var ser = serializer.startClass(writer, "Child", 1);
    super.serialize(writer, ser);
    ser.writeField("y", y);
    ser.endClass();
  }

  proc equals(other: borrowed Child) {
    return this.x == other.x &&
           this.y == other.y;
  }
}

proc test(type FormatWriter, type FormatReader) {
  var f = openMemFile();
  const rval = new R(5, 42.0);
  const gval = new G(real, int, 5.0, 42);
  const pval = new Parent(5);
  const cval = new Child(5, 42.0);

  proc print(str: string, val) {
    stdout.writeLiteral(str + ": ");
    stdout.withSerializer(FormatWriter).writeln(val);
  }

  proc helper(val, type T) {
    {
      f.writer().withSerializer(FormatWriter).write(val);
      print("wrote", val);
    }
    {
      var read = f.reader().withDeserializer(FormatReader).read(T);
      assert(val.equals(read));
      print("got", read);
    }
  }

  helper(rval, R);
  helper(gval, G(real,int));
  helper(pval, owned Parent);
  helper(cval, owned Child);
}

proc main() {
  test(DefaultSerializer, DefaultDeserializer);
  test(JsonSerializer, JsonDeserializer);
}
