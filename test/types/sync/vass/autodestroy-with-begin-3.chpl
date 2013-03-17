// Many sync variables, many intermediate calls/functions -
// before as well as within the 'begin'. Additional nested 'begin' as a bonus.

var barrier: sync int;
tt1();

proc tt1() {
  tt2();
  barrier = 35;
}

proc tt2() {
  var ss2: sync int = 46;
  tt3a(ss2);
}

proc tt3a(ss3a: sync int) {
  tt3b(ss3a);
}

proc tt3b(ss3b: sync int) {
  tt3c(ss3b);
}

proc tt3c(ss3c: sync int) {
  tt4(ss3c);
}

proc tt4(ss4: sync int) {
  begin {
    var sx4: sync int = 57;
    tt5a(ss4, sx4);
  }
}

proc tt5a(ss5a: sync int, sx5a: sync int) {
  tt5b(ss5a, sx5a);
}

proc tt5b(ss5b: sync int, sx5b: sync int) {
  tt5c(ss5b, sx5b);
}

proc tt5c(ss5c: sync int, sx5c: sync int) {
  tt6(ss5c, sx5c);
}

proc tt6(ss6: sync int, sx6: sync int) {
  var sy6: sync int = 68;
  tt7a(ss6, sx6, sy6);
}

proc tt7a(ss7a: sync int, sx7a: sync int, sy7a: sync int) {
  tt7b(ss7a, sx7a, sy7a);
}

proc tt7b(ss7b: sync int, sx7b: sync int, sy7b: sync int) {
  tt7c(ss7b, sx7b, sy7b);
}

proc tt7c(ss7c: sync int, sx7c: sync int, sy7c: sync int) {
  tt8(ss7c, sx7c, sy7c);
}

proc tt8(ss6: sync int, sx6: sync int, sy6: sync int) {
  // no, we are not done yet
  begin {
    tt9a(ss6, sx6, sy6);
  }
}

proc tt9a(ss9a: sync int, sx9a: sync int, sy9a: sync int) {
  tt9b(ss9a, sx9a, sy9a);
}

proc tt9b(ss9b: sync int, sx9b: sync int, sy9b: sync int) {
  tt9c(ss9b, sx9b, sy9b);
}

proc tt9c(ss9c: sync int, sx9c: sync int, sy9c: sync int) {
  ttLast(ss9c, sx9c, sy9c);
}

proc ttLast(ssLast: sync int, sxLast: sync int, syLast: sync int) {
  writeln(barrier);
  writeln(ssLast.isFull);
  writeln(sxLast.isFull);
  writeln(syLast.isFull);
}
