extern record myExternRecord {
  var x: int;
}

proc =(ref a: myExternRecord, b: int) {
  a.x = b;
}

proc _cast(type t: myExternRecord, b: int) {
  var tmp = new myExternRecord(b);
}

var ex0: myExternRecord;
var ex1: myExternRecord;
ex0;
ex1; // avoiding copy-init
ex0 = 1;
writeln(ex0);
ex1 = ex0;
writeln(ex1);
