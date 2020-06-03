class C {
  var x:int;
}

proc getC type {
  return borrowed C;
}

var x = new borrowed C(10);
writeln(x);

var y = new getC(10);
writeln(y);
