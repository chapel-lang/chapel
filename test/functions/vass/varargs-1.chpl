
var A111: [1..2] int = 101, i222 = 102;

proc show(tag, var11, var22) {
  writeln(tag, " :  ", var11, "  ", var22);
}

proc main {
  test_param("arg1 ", "arg2 ");
  test_param_typed("arg1 ", "arg2 ");
  test_param_queried("arg1 ", "arg2 ");
  show(1000, A111, i222);
  test_default(A111, 71);
  test_const(A111, 72);
  test_in(A111, i222);
  test_const_in(A111, 73);
  test_out(A111, i222);
  test_inout(A111, i222);
  test_ref(A111, i222);
  test_const_ref(A111, i222);
  show(9999, A111, i222);
}

proc test_param(param xxx...) {
  compilerWarning(xxx(1));
  compilerWarning(xxx(2));
  compilerWarning((...xxx));
  compilerWarning(xxx.size:string);
  show(81, xxx(1), xxx(2));
}

proc test_param_typed(param xxx:string...) {
  compilerWarning(xxx(1));
  compilerWarning(xxx(2));
  compilerWarning((...xxx));
  compilerWarning(xxx.size:string);
  show(82, xxx(1), xxx(2));
}

proc test_param_queried(param xxx...?nnn) {
  compilerWarning(xxx(1));
  compilerWarning(xxx(2));
  compilerWarning((...xxx));
  if nnn == xxx.size then
    compilerWarning("YES!!!");
  else
    compilerWarning("noooo");
  show(83, xxx(1), xxx(2));
}

proc test_default(xxx...) {
  show(1001, xxx(1), xxx(2));
  xxx(1) = 105;
  show(1009, xxx(1), xxx(2));
}

proc test_const(const xxx...) {
  show(2001, xxx(1), xxx(2));
}

proc test_in(in xxx...) {
  show(3001, xxx(1), xxx(2));
  xxx(1) = 301;
  xxx(2) = 302;
  show(3009, xxx(1), xxx(2));
}

proc test_const_in(const in xxx...) {
  show(4001, xxx(1), xxx(2));
}

proc test_out(out xxx...) {
  show(5001, xxx(1), xxx(2));
  xxx(1) = 501;
  xxx(2) = 502;
  show(5009, xxx(1), xxx(2));
}

proc test_inout(inout xxx...) {
  show(6001, xxx(1), xxx(2));
  xxx(1) = 601;
  xxx(2) = 602;
  show(6009, xxx(1), xxx(2));
}

proc test_ref(ref xxx...) {
  show(7001, xxx(1), xxx(2));
  xxx(1) = 701;
  xxx(2) = 702;
  show(7009, xxx(1), xxx(2));
}

proc test_const_ref(const ref xxx...) {
  show(8001, xxx(1), xxx(2));
}
