class Generic {
  var x;
}

var inst = new owned Generic(10);
writeln(isSubtype(inst.type, Generic));
writeln(isProperSubtype(inst.type, Generic));
