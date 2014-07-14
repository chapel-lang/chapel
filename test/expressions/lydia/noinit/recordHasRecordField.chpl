record aRecord {
  var namingIsHard: bool;
}

record foo {
  var t: aRecord;
}
var bar: foo = noinit;
bar.t = new aRecord();
writeln(bar);
