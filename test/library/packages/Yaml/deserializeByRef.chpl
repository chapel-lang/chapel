
use IO, List, Map, FormatHelper;

record A : serializable {
  var x: int;
  var b: B;

  proc serialize(writer, ref serializer) throws {
    ChapelIO.serializeDefaultImpl(writer, serializer, this);
  }

  proc ref deserialize(reader: fileReader(?), ref deserializer: reader.deserializerType) {
    writeln("IN A.deserialize");
    var des = deserializer.startRecord(reader, "A");
    des.readField("x", this.x);
    des.readField("b", this.b);
    des.endRecord();
  }

  proc init(reader: fileReader(?), ref deserializer) {
    writeln("IN A.init");
    var des = deserializer.startRecord(reader, "A");
    this.x = des.readField("x", int);
    this.b = des.readField("b", B);
    des.endRecord();
  }

  proc init() {
    this.x = 0;
    this.b = new B();
  }

  proc init(x: int, b: B) {
    this.x = x;
    this.b = b;
  }
}

record B : serializable {
  var t: (int, real);

  proc serialize(writer, ref serializer) throws {
    ChapelIO.serializeDefaultImpl(writer, serializer, this);
  }

  proc ref deserialize(reader: fileReader(?), ref deserializer: reader.deserializerType) {
    writeln("IN B.deserialize");
    var des = deserializer.startRecord(reader, "B");
    des.readField("t", this.t);
    des.endRecord();
  }

  proc init(reader: fileReader(?), ref deserializer) {
    writeln("IN B.init");
    var des = deserializer.startRecord(reader, "B");
    this.t = des.readField("t", (int, real));
    des.endRecord();
  }

  proc init() {
    this.t = (0, 0.0);
  }

  proc init(t: (int, real)) {
    this.t = t;
  }
}

record myList : serializable {
  var values: list(A);

  proc ref deserialize(reader: fileReader(?), ref deserializer: reader.deserializerType) {
    writeln("IN myList.deserialize");
    var des = deserializer.startList(reader);

    while des.hasMore() {
      this.values.pushBack(new A());
      des.readElement(this.values.last);
    }

    des.endList();
  }

  proc init(reader: fileReader(?), ref deserializer) {
    writeln("IN myList.init");
    init this;
    var des = deserializer.startList(reader);

    while des.hasMore() do
      this.values.pushBack(des.readElement(A));

    des.endList();
  }

  proc init(in values: list(A)) do this.values = values;

  proc serialize(writer: fileWriter(?), ref serializer) do
    writer.write(values);
}

record myMap : serializable {
  var values: map(int, A);

  proc ref deserialize(reader: fileReader(?), ref deserializer: reader.deserializerType) {
    writeln("IN myMap.deserialize");
    var des = deserializer.startMap(reader);

    while des.hasMore() {
      var k: int, v: A;
      des.readKey(k);
      des.readValue(v);
      this.values.add(k, v);
    }

    des.endMap();
  }

  proc init(reader: fileReader(?), ref deserializer) {
    writeln("IN myMap.init");
    init this;
    var des = deserializer.startMap(reader);

    while des.hasMore() do
      this.values.add(des.readKey(int), des.readValue(A));

    des.endMap();
  }

  proc init(in values: map(int, A)) do this.values = values;

  proc serialize(writer: fileWriter(?), ref serializer) do
    writer.write(values);
}

proc test(x, ref y) {
  printDebugFmt(x);
  var f = openTempFile();
  try {
    f.writer(locking=false).withSerializer(FormatWriter).write(x);
  } catch e {
    writeln("ERROR: ", e);
  }
  try {
    f.reader(locking=false).withDeserializer(FormatReader).read(y);

    if x != y {
      writeln("FAILURE: ", x.type:string);
      writeln("GOT: ", y);
    } else
      writeln("SUCCESS: ", x.type:string);
  } catch e {
    writeln("ERROR: ", e);
  }
}

proc main() {
  var l = new list([
    new A(1, new B((11, 1.1))),
    new A(2, new B((22, 2.2))),
    new A(3, new B((33, 3.3))),
  ]);

  var m = new map(int, A);
  for (a, i) in zip(l, 1..) do m.add(i, a);

  test(new myList(l), new myList(new list(A)));
  test(new myMap(m), new myMap(new map(int, A)));
}
