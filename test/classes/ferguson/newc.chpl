class C {
  var x:int;
}

proc getC type {
  return owned C;
}

var x = new owned C(10);
writeln(x);

var y = new getC(10);
writeln(y);
