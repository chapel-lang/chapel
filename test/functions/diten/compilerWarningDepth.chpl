proc f1(param d) {
  f2(d);
}

proc f2(param d) {
  f3(d);
}

proc f3(param d) {
  f4(d);
}

proc f4(param d) {
  compilerWarning("warning from f4 with d = " + d:string, d);
}

f1(0);  // line = 14
f1(1);  // line = 10
f1(2);  // line = 6
f1(3);  // line = 2
f1(4);  // line = 21
f1(5);  // Warning about exceeding callstack plus warning line = 1
f1(-1); // Warning about negative depth plus warning line = 14

f5();

proc f5() {
  compilerWarning("warning from f5 with default depth");
}
