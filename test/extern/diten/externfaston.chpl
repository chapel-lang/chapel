use CommDiagnostics;
require "externfaston.c", "externfaston.h";

pragma "fast-on safe extern function"
extern proc foo1(): int;
extern proc foo2(): int;

proc bar(n, param fooVersion) {
  if fooVersion == 1 then
    return n + foo1();
  else
    return n + foo2();
}

startVerboseComm();
on Locales(here.id + 1 % numLocales) {
  bar(2, 1);
}

on Locales(here.id + 1 % numLocales) {
  bar(2, 2);
}
stopVerboseComm();
