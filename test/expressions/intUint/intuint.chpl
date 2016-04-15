{
  use IntUIntOpsInt;

  var i: int = 1;
  var u: uint = 0;

  var t = i + u;
  var t2 = u + i;

  writeln("t = ", t, ": ", t.type:string);
  writeln("t2 = ", t2, ": ", t2.type:string);
}

{
  use IntUIntOpsUInt;

  var i: int = 1;
  var u: uint = 0;

  var t = i + u;
  var t2 = u + i;

  writeln("t = ", t, ": ", t.type:string);
  writeln("t2 = ", t2, ": ", t2.type:string);
}

{
  use IntUIntOpsFirst;

  var i: int = 1;
  var u: uint = 0;

  var t = i + u;
  var t2 = u + i;

  writeln("t = ", t, ": ", t.type:string);
  writeln("t2 = ", t2, ": ", t2.type:string);
}
