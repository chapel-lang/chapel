// 2024-01 Vass: this test locks in the existing behavior.
// Today's behavior for f2 and f3 is questionable,
// especially that canResolve("f3"...) aborts compilation,
// so feel free to improve.

use Reflection;

proc f1(x: int) do
  compilerError("fatal error in f1()");

proc f2(x: int) do
  const y: int(8) = x;  // error: int(8) <-- int(64)

proc f3(x: int) do
  f1(3,4);              // error: f1 accepts only one argument

if canResolve("f1", 5)
  then compilerWarning("YESSS, f1() resolves");
  else compilerWarning("NOOOO, f1() does not resolve");

if canResolve("f2", 5)
  then compilerWarning("YESSS, f2() resolves");
  else compilerWarning("NOOOO, f2() does not resolve");

if canResolve("f3", 5)
  then compilerWarning("YESSS, f3() resolves");
  else compilerWarning("NOOOO, f3() does not resolve");

compilerError("SUCCESSFULL COMPILATION");
