// #18565

config param testNum = 0;

class C {
  proc foo() {
  }

  proc bar(x: int) {
  }
}

class D {
  proc foo() {
  }

  proc bar(x: real) {
  }
}

class E {
  proc foo() {
  }

  proc bar(x: string) {
  }
}

record S {
  proc foo(x: int) {
  }

  proc bar() {
  }
}

proc int.foo() {
}

proc int.bar(x: bool) {
}

var myS: S;

if testNum == 1 then myS.foo();
if testNum == 2 then myS.bar(1);
