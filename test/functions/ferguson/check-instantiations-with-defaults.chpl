proc default1() {
  return 1;
}

proc default2(arg) {
  return 1+arg;
}

proc f(arg:? = default1(), b:arg.type = default2(arg)) {
  compilerWarning(arg.type:string, " ", b.type:string, errorDepth=0);
}

config param case = 1;

proc main() {
  if case == 1 {
    f();
    f(1);
    f(1,2);
  } else if case == 2 {
    f();
    f(1,2);
    f(1);
  } else if case == 3 {
    f(1);
    f();
    f(1,2);
  } else if case == 4 {
    f(1);
    f(1,2);
    f();
  } else if case == 5 {
    f(1,2);
    f();
    f(1);
  } else if case == 6 {
    f(1,2);
    f(1);
    f();
  }
}
