use ParallelIO, IO, JSON, List;

config const n = 1000,
             nTasks=8;

record R: serializable {
  var x: int;
  var y: real;
}

proc main() {
  const fileName = "data.json";

  var l1 = createRandomJsonFile(n, fileName);
  var l2 = readParallelRecords(fileName);

  for (j1, j2) in zip(l1, l2) {
    assert(j1.x == j2.x);
    assert(abs(j1.y - j2.y) < 1e-6);
  }
}

proc createRandomJsonFile(numRecords: int, fileName: string): list(R) {
  use Random;
  var rng = new RandomStream(real);

  proc type R.rand(): R {
    const x = rng.getNext(int),
          y = rng.getNext(real);

    return new R(x, y);
  }

  var f = open(fileName, ioMode.cw),
      w = f.writer(serializer = new jsonSerializer());

  const header = "some json data: ";
  w.withSerializer(new defaultSerializer()).writeln(header);

  var l = new list(R);
  for 1..numRecords {
    const r = R.rand();
    l.pushBack(r);
    w.writeln(r);
  }

  return l;
}

proc readParallelRecords(fileName: string): list(R) {
  var f = open(fileName, ioMode.r);
  const values = readParallelLocal(f, R, nTasks, skipHeaderBytes=-1, jsonDeserializer);
  return values;
}

proc R.init() {
  this.x = 0;
  this.y = 0;
}

proc R.init(x: int, y:real) {
  this.x = x;
  this.y = y;
}

proc R.init(reader: fileReader(?), ref deserializer) throws {
  init this;
  var r = new R();
  r.deserialize(reader, deserializer);
  this.x = r.x;
  this.y = r.y;
}

proc ref R.deserialize(reader: fileReader(?), ref deserializer) throws {
  var des = deserializer.startRecord(reader, "R");
  this.x = des.readField("x", int);
  this.y = des.readField("y", real);
  des.endRecord();
}

proc R.serialize(writer: fileWriter(?), ref serializer) throws {
  var ser = serializer.startRecord(writer, "R", 2);
  ser.writeField("x", this.x);
  ser.writeField("y", this.y);
  ser.endRecord();
}
