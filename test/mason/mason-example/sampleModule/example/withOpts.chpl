
config param exParam1: int;
config param exParam2: string;
config const exConfig: string;

proc testConfigConst() {

  writeln(exConfig);
  writeln(exParam1);
  writeln(exParam2);
}

proc main() {
  testConfigConst();
}

