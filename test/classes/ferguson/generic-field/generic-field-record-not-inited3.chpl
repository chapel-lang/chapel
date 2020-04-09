record GenericRecord {
  var field;
}

record Wrapper {
  var f:GenericRecord;
  proc init(f : GenericRecord) {
    this.f = f;
  }
}

proc test1() {
  var z:Wrapper(GenericRecord(int));

  writeln(z.type:string, " ", z);
}

test1();
