pragma "use default init"
record GenericRecord {
  var field;
}

record Wrapper {
  var f:GenericRecord;
  proc init(type recType) {
    var tmp:recType;
    this.f = tmp;
  }
}

proc test1() {
  var z:Wrapper(GenericRecord(int));

  writeln(z.type:string, " ", z);
}

test1();
