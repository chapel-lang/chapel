class C {
  var x:int;
}

proc getC() type {
  return borrowed C;
}

var x = new unmanaged C(10);
writeln(x);

var y = new unmanaged getC()(10);
writeln(y);

delete y;
delete x;
