record MyRecord {
  param myparam:bool;
  var x:int;
  proc myfunc() { return x; }
}


record SubRecord : MyRecord(true) {
  proc myfunc2() { return x + 1; }
}

var r = new MyRecord(myparam=true, x = 10);
var r2 = new SubRecord(x = 20);

writeln(r.myfunc());
writeln(r2.myfunc());
writeln(r2.myfunc2());
