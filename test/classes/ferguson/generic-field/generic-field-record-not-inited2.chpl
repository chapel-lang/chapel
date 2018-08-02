record GenericRecord {
  var field;
}

record Wrapper {
  type t;
  var f:GenericRecord(t);
  proc init(type recType) {
    //var tmp:recType;
    //this.f = tmp;
    t = recType;
  }
}

proc test1() {
  var z:Wrapper(GenericRecord(int));

  writeln(z.type:string, " ", z);
}

test1();
