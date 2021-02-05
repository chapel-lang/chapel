config param testNum = 1;

// C defines one foo(), two bar()s and no baz()s
class C {
  proc foo(x: string) {
  }

  proc bar(x: string) {
  }

  proc bar(x: D) {
  }

  proc test() {
    if testNum == 10 then
      this.foo(42);
    if testNum == 11 then
      foo(42);
    if testNum == 12 then
      this.bar(42);
    if testNum == 13 then
      bar(42);
    if testNum == 14 then
      this.baz(42);
    if testNum == 15 then
      baz(42);
  }
}


// D defines one foo(), one bar, and one baz
class D {
  proc foo(x: C) {
  }

  proc bar(x: int) {
  }

  proc baz(x: C) {
  }

  proc test() {
    if testNum == 16 then
      this.foo("hi");
    if testNum == 17 then
      foo("hi");
    if testNum == 18 then
      this.bar("hi");
    if testNum == 19 then
      bar("hi");
    if testNum == 20 then
      this.baz("hi");
    if testNum == 21 then
      baz("hi");
  }
}

// standalone, we have two foos, one bar, one baz
proc foo(x: C) {
}

proc foo(x: D) {
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

  if testNum >= 10 && testNum <= 15 then
    myC.test();

  if testNum >= 16 then
    myD.test();
}
