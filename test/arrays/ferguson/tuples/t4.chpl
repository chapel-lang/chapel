var A = [1,2,3,4];
var A2 = (A,A);

proc foo() : A2.type
{
  return (A,A);
}

proc bar(tup) {
  tup(1)[1] = 2;
  writeln(tup(2)[1]);
}

proc run() {
  bar(foo());
}

run();
