proc getTupArrayType() type {
  var A:[1..4] int;
  var tup = (A,);
  return tup.type;
}

proc foo() : getTupArrayType() //1* ([1..4] int)
{
  var A = [1,2,3,4];
  return (A,);
}

proc bar(tup) {
  tup(1)[1] = 2;
  writeln(tup(1)[1]);
}

proc run() {
  bar(foo());
}

run();
