class C {
  var x:int;
}

proc getC type {
  return borrowed C;
}

var x = (new owned C(10)).borrow();
writeln(x);

var y = new getC(10);
writeln(y);
