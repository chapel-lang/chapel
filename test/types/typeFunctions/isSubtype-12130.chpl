class Generic {
  var x;
}

var inst = (new owned Generic(10)).borrow();
writeln(isProperSubtype(inst.type, Generic));
writeln(isProperSubtype(Generic, inst.type));

writeln(isProperSubtype(inst.type, borrowed Generic));
writeln(isProperSubtype(borrowed Generic, inst.type));
