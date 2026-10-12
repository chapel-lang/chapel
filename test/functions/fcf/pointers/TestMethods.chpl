
proc chpl_anyProc.foobar() {
  writeln("foobar");
}

proc procA() {
  return 5;
}

proc procB() {
  return 10;
}

proc test(cond: bool) {
  var x = if cond then procA else procB;
  x.foobar();
}

proc multi(x: int, y: real, z: string) {
  writeln("multi");
}

proc main() {
  procA.foobar();
  test(true);
  writeln(test.retType:string);
  writeln(procA.retType:string);

  writeln(test.argTypes:string);
  writeln(multi.argTypes:string);

  // test for .serialize
  writeln(multi);
}
