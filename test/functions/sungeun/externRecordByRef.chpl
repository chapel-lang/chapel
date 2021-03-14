extern record myExternRecord {
  var x: int;
}

proc initR(ref a: myExternRecord) {
  a.x = -1;
}

var ex0: myExternRecord;
initR(ex0);
writeln(ex0);

