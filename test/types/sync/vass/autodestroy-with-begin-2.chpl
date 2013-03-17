// 2 sync variables, 1 intermediate call/function.
// Could be simplified further by hanving t2 invoke t4 directly.

var barrier: sync int;
tt1();

proc tt1() {
  tt2();
  barrier = 35;
}

proc tt2() {
  var ss2: sync int = 46, qq2: sync int = 47;
  tt3(ss2, q2);
}

proc tt3(ss3: sync int, qq3: sync int) {
  tt4(ss3, qq3);
}

proc tt4(ss4: sync int, qq4: sync int) {
  writeln(ss4.isFull);
  begin {
    writeln(barrier);
    writeln(qq4.isFull);
  }
}
