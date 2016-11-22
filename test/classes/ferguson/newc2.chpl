class C {
  var x:int;
}

proc getC() type {
  return C;
}

var x = new C(10);
writeln(x);

var y = new getC()(10);
writeln(y);

delete y;
delete x;
