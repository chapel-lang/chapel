record GenericRecord {
  var field;
}

record Wrapper {
  type t;
  var f:GenericRecord(t);
  proc init(type t) {
    //var tmp:recType;
    //this.f = tmp;
    this.t = t;
  }
}

proc test1() {
  var z:Wrapper(GenericRecord(int));

  writeln(z.type:string, " ", z);
}

test1();
