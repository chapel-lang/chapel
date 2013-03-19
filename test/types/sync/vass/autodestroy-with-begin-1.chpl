// 1 sync variable, 1 intermediate call/function.
// Could be simplified further by hanving t2 invoke t4 directly.

var barrier: sync int;
tt1();

proc tt1() {
  tt2();
  barrier = 35;
}

proc tt2() {
  var ss2: sync int = 46;
  tt3(ss2);
}

proc tt3(ss3: sync int) {
  tt4(ss3);
}

proc tt4(ss4: sync int) {
  begin {
    writeln(barrier);
    writeln(ss4.isFull);
  }
}
