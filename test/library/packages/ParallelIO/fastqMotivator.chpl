use ParallelIO, IO, Time;

config const nTasks = 1,
             fileName = "data.fastq";

config param collectPerfData = false;

proc main() {
  const seqTruth = serialReadFastQ(fileName);
  var t = new stopwatch();

  if collectPerfData then t.start();
  const s1 = readParallelLocal(fileName, fastQSequence, nTasks);
  if collectPerfData {
    t.stop();
    writeln("parallel_local: ", t.elapsed());
  }

  if collectPerfData then t.restart();
  const s2 = readParallelDelimitedLocal(fileName, delim=b"@", fastQSequence, delimInclusive=true, nTasks);
  if collectPerfData {
    t.stop();
    writeln("parallel_delimited_local: ", t.elapsed());
  }

  if collectPerfData then t.restart();
  const s3 = readParallel(fileName, fastQSequence, nTasks);
  if collectPerfData {
    t.stop();
    writeln("parallel: ", t.elapsed());
  }

  if collectPerfData then t.restart();
  const s4 = readParallelDelimited(fileName, delim=b"@", fastQSequence, delimInclusive=true, nTasks);
  // const s4 = serialReadFastQ(fileName);
  if collectPerfData {
    t.stop();
    writeln("parallel_delimited: ", t.elapsed());
  }

  var q = 0;
  for s in (s1, s2, s3, s4) {
    assert(s.size == seqTruth.size, "size mismatch");
    for i in s.domain {
      // assert(s[i] == seqTruth[i], "sequence mismatch at: " + i:string);
      assert(s[i].seqID == seqTruth[i].seqID, "seqID mismatch at: " + i:string);
      assert(s[i].seq == seqTruth[i].seq, "seq mismatch at: " + i:string);
      assert(s[i].desc == seqTruth[i].desc, "desc mismatch at: " + i:string);
      if s[i].qualities != seqTruth[i].qualities then
        writeln("qualities mismatch at: ", i:string, "\t", q, "\na: ", s[i].qualities, "\nb: ", seqTruth[i].qualities);

      assert(s[i].qualities == seqTruth[i].qualities, "qualities mismatch at: " + i:string);
    }
    q += 1;
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
