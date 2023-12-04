use IO;

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
