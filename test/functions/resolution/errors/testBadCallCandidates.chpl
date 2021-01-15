config param testNum = 1;

class C {
  proc foo(x: string) {
  }

  proc bar(x: string) {
  }

  proc test() {
    this.foo(42);
    foo(42);
  }
}

class D {
  proc foo(x: int) {
  }

  proc type bar(x: int) {
  }

  proc test() {
    this.bar("hi");
    bar("hi");
  }
}


proc C.baz(x: string) {
}

proc type D.baz(x: int) {
}

proc foo(x: C) {
}

proc bar(x: D) {
}

proc baz(x: C) {
}

proc main() {
  var myC = new C();
  var myD = new D();

  if testNum == 1 then
    myC.foo(42);

  if testNum == 2 then
    myD.foo("hi");

  if testNum == 3 then
    foo(42);

  if testNum == 4 then
    myC.bar(42);

  if testNum == 5 then
    myD.bar("hi");

  if testNum == 6 then
    bar("hi");

  if testNum == 7 then
    myC.baz(42);

  if testNum == 8 then
    myC.baz("hi");

  if testNum == 9 then
    baz(42);

  if testNum == 10 then
    myC.test();

  if testNum == 11 then
    myD.test();
}
