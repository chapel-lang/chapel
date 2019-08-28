class C {
  var x:int;
}

proc getC() type {
  return unmanaged C;
}

var x = new unmanaged C(10);
writeln(x);

var y = new getC()(10);
writeln(y);

delete y;
delete x;
