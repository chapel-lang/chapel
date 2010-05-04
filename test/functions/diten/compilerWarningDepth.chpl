def f1(param d) {
  f2(d);
}

def f2(param d) {
  f3(d);
}

def f3(param d) {
  f4(d);
}

def f4(param d) {
  compilerWarning("warning from f4 with d = " + d, d);
}

f1(0);  // line = 14
f1(1);  // line = 10
f1(2);  // line = 6
f1(3);  // line = 2
f1(4);  // line = 21
f1(5);  // line = 1 (module init function)
f1(6);  // Warning about exceeding callstack plus warning line = 1
f1(-1); // Warning about negative depth plus warning line = 14

f5();

def f5() {
  compilerWarning("warning from f5 with default depth");
}
