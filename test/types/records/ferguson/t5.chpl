record MyRecord {
  const myparam:bool;
  var x:int;
  proc myfunc() { return x; }
}


record SubRecord : MyRecord {
  proc myfunc2() { return x + 1; }
}

proc main() {

  var r = new MyRecord(myparam=true, x = 10);
  var r2 = new SubRecord(myparam=true, x = 20);

  writeln(r.myfunc());
  writeln(r2.myfunc());
  writeln(r2.myfunc2());

}
