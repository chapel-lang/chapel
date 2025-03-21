use Print;

proc test0() {
  var x1: int;
  var x2: real;
  var x3: (int, real);
  println(x1);
  println(x2);
  println(x3);
}

proc test1() {
  var x1 = if true then 0 else 1;
  println(x1);

  var x2 = if false then 0 else 1;
  println(x2);

  var flag1 = true;
  // TODO: If I misname 'flag1' to 'flag' then LookupHelper in
  //       'scope-queries.cpp' will explode on an assert...
  var x3 = if flag1 then 2 else 3;
  println(x3);

  var flag2 = false;
  var x4 = if flag2 then 2 else 3;
  println(x4);
}

proc test2() {
  proc printTheVarargs(x...?n) {
    println(x.size);
    println(x);
  }

  printTheVarargs(8, 2.3);
}

proc main() {
  test0();
  test1();
  test2();
}
