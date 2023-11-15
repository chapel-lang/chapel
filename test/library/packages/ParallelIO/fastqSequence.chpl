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

  // this initializer must throw when a sequence cannot be deserialized
  // thus deserialization is done in phase 2 (after "init this")
  proc init(reader: fileReader(?), ref deserializer) throws {
    init this;
    var x = new fastQSequence();
    x.deserialize(reader, deserializer);
    this.seqID = x.seqID;
    this.seq = x.seq;
    this.desc = x.desc;
    this.qualities = x.qualities;
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
