pragma "use default init"
record GenericRecord {
  var field;
}

pragma "use default init"
record Wrapper {
  var f:GenericRecord;
}

proc test1() {
  var z:Wrapper(GenericRecord(int));

  writeln(z.type:string, " ", z);
}

test1();
