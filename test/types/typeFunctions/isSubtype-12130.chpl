class Generic {
  var x;
}

var inst = (new owned Generic(10)).borrow();
writeln(inst.type < Generic);
writeln(inst.type > Generic);

writeln(inst.type < borrowed Generic);
writeln(inst.type > borrowed Generic);
