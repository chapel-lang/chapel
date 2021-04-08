extern record myExternRecord {
  var x: int;
}

operator =(ref a: myExternRecord, b: int) {
  a.x = b;
}

operator :(b:int, type t: myExternRecord) {
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
