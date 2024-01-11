use ParallelIO, IO, Time;

config const nTasks = 1,
             fileName = "data.fastq";

config param CollectPerfData = false;

proc main() {
  var sw = new stopwatch();

  if CollectPerfData {
    // warmup
    serialReadFastQ(fileName);
    sw.start();
  }
  const seqTruth = serialReadFastQ(fileName);
  if CollectPerfData then writeln("serial: ", sw.elapsed());

  if CollectPerfData then sw.restart();
  const s1 = readItemsAsBlockArray(fileName, fastQSequence, b"\n@", nTasks=nTasks);
  if CollectPerfData then writeln("parallel: ", sw.elapsed());

  if CollectPerfData then sw.restart();
  const s2 = readItemsAsArray(fileName, fastQSequence, b"\n@", nTasks=nTasks);
  if CollectPerfData then writeln("parallel local: ", sw.elapsed());

  for s in (s1, s2) {
    assert(s.size == seqTruth.size, "size mismatch");
    for i in s.domain do
      assert(s[i] == seqTruth[i], "sequence mismatch at: " + i:string);
  }
}

record fastQSequence: serializable {
  var seqID: string;
  var seq: bytes;
  var desc: string;
  var qualities: bytes;

  proc init() {
    this.seqID = "";
    this.seq = b"";
    this.desc = "";
    this.qualities = b"";
  }

  proc ref deserialize(reader: fileReader(?), ref deserializer) throws {
    reader.readLiteral(b"@", ignoreWhitespace=false);
    const ID   = reader.readLine(string, stripNewline=true);
    const seq  = reader.readLine(bytes, stripNewline=true);

    // extra assurance that the sequence is valid
    if seq.count(b"@") > 0 || seq.count(b"+") > 0 then
      throw new Error("Invalid sequence");

    reader.readLiteral(b"+", ignoreWhitespace=false);
    const desc = reader.readLine(string, stripNewline=true);
    const qual = reader.readLine(bytes, stripNewline=true);

    this.seqID = ID;
    this.seq = seq;
    this.desc = desc;
    this.qualities = qual;
  }

  proc serialize(writer: fileWriter(?), ref serializer) throws {
    var ser = serializer.startRecord(writer, "sequence", 4);
    ser.writeField("seqID", this.seqID);
    ser.writeField("seq", this.seq);
    ser.writeField("desc", this.desc);
    ser.writeField("qualities", this.qualities);
    ser.endRecord();
  }
}

proc serialReadFastQ(path: string): [] fastQSequence {
  use List;

  var f = open(path, ioMode.r),
      r = f.reader(locking=false),
      l = new list(fastQSequence);

  var seq: fastQSequence;
  while r.read(seq) do
    l.pushBack(seq);

  return l.toArray();
}
