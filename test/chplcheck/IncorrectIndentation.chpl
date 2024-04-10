proc f1()
{
  writeln("hi");
  writeln("??");
}

proc f2()
{
writeln("hi");
}

proc f3() {
writeln("hi");
}

proc f4() {
  writeln("hi");
    writeln("hi");
}

proc f5() {
  writeln("hi"); writeln("hi");
}

proc f6() {
  for 1..10 do
  writeln("hi");
}

module M1
{
  writeln("hi");
  writeln("??");
}

module M2
{
writeln("hi");
}

module M3 {
writeln("hi");
}

module M4 {
  writeln("hi");
    writeln("hi");
}

module M5 {
  writeln("hi"); writeln("hi");
}

module M6 {
  for 1..10 do
  writeln("hi");
}

for 1..10 {
  writeln("hi");
}

for 1..10
{
writeln("hi");
}

for 1..10 {
writeln("hi");
}

module NestedOuter {
  module NestedInner {
    writeln("hi");
     writeln("??");
    writeln("??");
    record nestedRecord {
      proc firstProc() {}
       proc secondProc() {}
      proc thirdProc() {}

      proc nestedProcOuter() {
        proc nestedProcInner(x: int) do return x;
          proc nestedProcInner(x: string) {
            writeln(x);
             writeln(x);
            writeln(x);
            return x;
          }
      }
    }
  }
}
