
use IO;
use Json;

record R {
  var x : int;
  var y : real;

  proc init(x: int = 0, y: real = 0.0) {
    this.x = x;
    this.y = y;
  }

  proc init(reader: fileReader, ref deserializer) {
    const ref r = reader;
    ref fmt = r.deserializer;
    fmt.startRecord(r, "R", 2);
    this.x = fmt.deserializeField(r, "x", int);
    this.y = fmt.deserializeField(r, "y", real);
    fmt.endRecord(r);
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
    const ref r = reader;
    ref fmt = r.deserializer;
    fmt.startRecord(r, "G", 2);
    this.x = fmt.deserializeField(r, "x", A);
    this.y = fmt.deserializeField(r, "y", B);
    fmt.endRecord(r);
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
    const ref r = reader;
    ref fmt = r.deserializer;
    fmt.startClass(r, "Parent", 1);
    this.x = fmt.deserializeField(r, "x", int);
    fmt.endClass(r);
  }

  override proc serialize(writer: fileWriter, ref serializer) {
    serializer.startClass(writer, "Parent", 1);
    serializer.serializeField(writer, "x", x);
    serializer.endClass(writer);
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
    const ref r = reader;
    ref fmt = r.deserializer;
    fmt.startClass(r, "Child", 1);
    super.init(r, deserializer);
    this.y = fmt.deserializeField(r, "y", real);
    fmt.endClass(r);
  }

  override proc serialize(writer: fileWriter, ref serializer) {
    serializer.startClass(writer, "Child", 1);
    super.serialize(writer, serializer);
    serializer.serializeField(writer, "y", y);
    serializer.endClass(writer);
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
