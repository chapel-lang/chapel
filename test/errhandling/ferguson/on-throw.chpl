use ExampleErrors;

proc test() throws {

  for i in 1..10 {
    on Locales[numLocales-1] {
      writeln(i);
      if i == 4 then
        throw new owned StringError("test");
    }
  }
}

try {
  writeln("before test");
  test();
  writeln("after test");
} catch {
  writeln("caught error");
}
