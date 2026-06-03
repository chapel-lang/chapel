export "foo" proc foobarbaz(doHalt: bool) {
  writeln('Hello!');
  if doHalt then halt('Triggering unwind!');
}

proc test1() {
  extern "foo" proc helperTest1(doHalt: bool): void;
  helperTest1(doHalt=false);
  test2();
} 

proc test2() {
  extern "foo" proc helperTest2(doHalt: bool): void;
  helperTest2(doHalt=false);
  test3();
} 

proc test3() {
  extern "foo" proc helperTest3(doHalt: bool): void;
  helperTest3(doHalt=true);
} 

proc main() {
  test1();
}
