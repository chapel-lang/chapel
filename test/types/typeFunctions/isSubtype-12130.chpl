class Generic {
  var x;
}

var inst = new borrowed Generic(10);
writeln(inst.type < Generic);
writeln(inst.type > Generic);

writeln(inst.type < borrowed Generic);
writeln(inst.type > borrowed Generic);
