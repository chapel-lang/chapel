record GenericRecord { var field; }

proc f(type t, x: t) {
  writeln(t:string, " ", x.type:string);
}

var x = new GenericRecord(1);
f(GenericRecord, x);
