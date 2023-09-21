class Generic {
  var x;
}

var ownInst = new owned Generic(10);
var inst = ownInst.borrow();
writeln(isProperSubtype(inst.type, Generic));
writeln(isProperSubtype(Generic, inst.type));

writeln(isProperSubtype(inst.type, borrowed Generic));
writeln(isProperSubtype(borrowed Generic, inst.type));
