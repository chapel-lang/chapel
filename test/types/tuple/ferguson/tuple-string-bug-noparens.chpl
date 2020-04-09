config const i = 1;
config const msg = "test " + i:string;

proc f {
  return (msg + i:string, true);
}

proc test1() {
  var (t, ok) = f;
  writeln(t);
  writeln(ok);
}

proc test2() {
  var tup = f;
  writeln(tup(0));
  writeln(tup(1));
}

proc test3() {
  var tup = f;
  var (t, ok) = tup;
  writeln(t);
  writeln(ok);
}



test1();
test2();
test3();
