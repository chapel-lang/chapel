var D = {1..10};
var A:[D] int;

proc returnADomain() {
  return A.domain;
}
proc test1() {
  writeln("test1");
  D = {1..10};
  var dom = returnADomain();
  D = {1..3};
  writeln(dom);
}
test1();

proc acceptDomainIn(in dom) {
  D = {1..3};
  writeln(dom);
}
proc test2() {
  writeln("test2");
  D = {1..10}; 
  acceptDomainIn(D);
}
test2();

proc test3() {
  writeln("test3");
  D = {1..10}; 
  acceptDomainIn(A.domain);
}
test3();

proc maybeReturnADomain(option: bool) {
  if option {
    return A.domain;
  } else {
    return {1..99};
  }
}
proc test4() {
  writeln("test4");
  D = {1..10}; 
  var dom = maybeReturnADomain(true);
  D = {1..3};
  writeln(dom);
}
test4();
proc test5() {
  writeln("test5");
  D = {1..10}; 
  var dom = maybeReturnADomain(false);
  D = {1..3};
  writeln(dom);
}
test5();

proc maybeReturnADomainSplit(option: bool) {
  var dom: domain(1);
  if option {
    dom = A.domain;
  } else {
    dom = {1..99};
  }
  return dom;
}
proc test6() {
  writeln("test6");
  D = {1..10}; 
  var dom = maybeReturnADomainSplit(true);
  D = {1..3};
  writeln(dom);
}
test6();
proc test7() {
  writeln("test7");
  D = {1..10}; 
  var dom = maybeReturnADomainSplit(false);
  D = {1..3};
  writeln(dom);
}
test7();

proc maybeReturnADomainSplitNoType(option: bool) {
  var dom;
  if option {
    dom = A.domain;
  } else {
    dom = {1..99};
  }
  return dom;
}
proc test8() {
  writeln("test8");
  D = {1..10}; 
  var dom = maybeReturnADomainSplit(true);
  D = {1..3};
  writeln(dom);
}
test8();
proc test9() {
  writeln("test9");
  D = {1..10}; 
  var dom = maybeReturnADomainSplit(false);
  D = {1..3};
  writeln(dom);
}
test9();

proc returnADomainCopyInit() {
  var dom: domain(1) = A.domain;
  return dom;
}
proc test10() {
  writeln("test10");
  D = {1..10}; 
  var dom = returnADomainCopyInit();
  D = {1..3};
  writeln(dom);
}
test10();

proc test11() {
  writeln("test11");
  D = {1..10};
  var dom = A.domain;
  D = {1..3};
  writeln(dom);
}
test11();

proc test12() {
  writeln("test12");
  D = {1..10};
  var tmp = A.domain;
  var dom = tmp;
  D = {1..3};
  writeln(dom);
}
test12();

proc test13() {
  writeln("test13");
  D = {1..10};
  const ref tmp = A.domain;
  var dom = tmp;
  D = {1..3};
  writeln(dom);
}
test13();
