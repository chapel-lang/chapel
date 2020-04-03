proc foo()
{
  var A = [1,2,3,4];
  return (A,A);
}

proc bar(tup) {
  tup(0)[0] = 2;
  writeln(tup(1)[0]);
}

proc run() {
  bar(foo());
}

run();
