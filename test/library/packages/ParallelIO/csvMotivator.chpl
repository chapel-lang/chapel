use ParallelIO, IO, Random, Time, FileSystem;

const fileName = "colors.csv";

config const nItems = 1000,
             nTasks = 4;

config param CollectPerfData = false;

proc main() {
  const cTruth = makeRandomCSVFile(fileName, nItems);

  var s = new stopwatch();

  if CollectPerfData {
    // warmup
    serialReadCSV(fileName, 1);
    s.start();
    serialReadCSV(fileName, 1);
    writeln(s.elapsed());
  }

  if CollectPerfData then s.restart();
  const c1 = readDelimitedAsBlockArray(fileName, t=color, nTasks=nTasks, header=headerPolicy.skipLines(1));
  if CollectPerfData then writeln(s.elapsed());

  if CollectPerfData then s.restart();
  const c2 = readDelimitedAsArray(fileName, t=color, nTasks=nTasks, header=headerPolicy.skipLines(1));
  if CollectPerfData then writeln(s.elapsed());

  // test correctness
  assert(c1.size == nItems);
  assert(c2.size == nItems);
  for c in (c1, c2) {
    for (j1, j2, i) in zip(cTruth, c, 0..) {
      if j1 != j2 {
        writeln("mismatch at ", i, ": [", j1, "] != [", j2, "]");
        break;
      }
    }
  }

  remove(fileName);
}

var rng = new randomStream(uint(8));

record color: serializable {
  var r, g, b: uint(8);

  proc init() {
    this.r = 0;
    this.g = 0;
    this.b = 0;
  }

  proc init(r: uint(8), g: uint(8), b: uint(8)) {
    this.r = r;
    this.g = g;
    this.b = b;
  }
};

proc ref color.deserialize(reader: fileReader(?), ref deserializer) throws {
  reader.read(this.r);
  reader.readLiteral(b",");
  reader.read(this.g);
  reader.readLiteral(b",");
  reader.read(this.b);
}

proc color.serialize(writer: fileWriter(?), ref serializer) throws {
  writer.write(this.r);
  writer.writeLiteral(b",");
  writer.write(this.g);
  writer.writeLiteral(b",");
  writer.write(this.b);
}

proc type color.random(): color {
  return new color(
    rng.next(),
    rng.next(),
    rng.next()
  );
}

proc makeRandomCSVFile(path: string, n: int): [] color throws {
  var f = open(path, ioMode.cwr),
      w = f.writer(locking=false),
      colors: [0..<n] color;

  w.writeln("r,g,b");

  for i in 0..<n {
    const c = color.random();
    w.writeln(c);
    colors[i] = c;
  }

  return colors;
}

proc serialReadCSV(path: string, nHeaderLines: int): [] color throws {
  use List;

  var f = open(path, ioMode.r),
      r = f.reader(locking=false),
      l = new list(color);

  for 0..#nHeaderLines do r.readLine();

  var c: color;
  while r.read(c) {
    l.pushBack(c);
    r.readLine();
  }

  return l.toArray();
}
