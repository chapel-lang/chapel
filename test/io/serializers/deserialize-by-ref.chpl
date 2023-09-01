
use IO, List, Map, FormatHelper;

record A {
  var x: int;
  var b: B;

  proc type deserializeFrom(reader: fileReader, ref deserializer) {
    var a = new A();

    var des = deserializer.startRecord(reader, "A");
    des.readField("x", a.x);
    des.readField("b", a.b);
    des.endRecord();

    return a;
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

record B {
  var t: (int, real);

  proc type deserializeFrom(reader: fileReader, ref deserializer) {
    var b = new B();

    var des = deserializer.startRecord(reader, "B");
    des.readField("t", b.t);
    des.endRecord();

    return b;
  }

  proc init() {
    this.t = (0, 0.0);
  }

  proc init(t: (int, real)) {
    this.t = t;
  }
}

record myList {
  var values: list(A);

  proc type deserializeFrom(reader: fileReader, ref deserializer) {
    var l = new list(A);

    var des = deserializer.startList(reader);
    while des.hasMore() {
      l.pushBack(new A());
      des.readElement(l.last);
    }

    des.endList();
    return new myList(l);
  }

  proc serialize(writer: fileWriter, ref serializer) {
    writer.write(values);
  }
}

record myMap {
  var values: map(int, A);

  proc type deserializeFrom(reader: fileReader, ref deserializer) {
    var m = new map(int, A);

    var des = deserializer.startMap(reader);
    while des.hasMore() {
      var k: int, v: A;
      des.readKey(k);
      des.readValue(v);
      m.add(k, v);
    }

    des.endMap();
    return new myMap(m);
  }

  proc serialize(writer: fileWriter, ref serializer) {
    writer.write(values);
  }
}

proc test(x) {
  printDebugFmt(x);
  var f = openMemFile();
  try {
    f.writer().withSerializer(FormatWriter).write(x);
  } catch e {
    writeln("ERROR: ", e);
  }
  try {
    var y = f.reader().withDeserializer(FormatReader).read(x.type);

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

  test(new myList(l));
  test(new myMap(m));
}
