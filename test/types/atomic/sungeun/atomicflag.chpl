var x: atomic bool;
if x.testAndSet() then
  writeln("set");
 else
   writeln("not set");
if x.testAndSet() then
  writeln("set");
 else
   writeln("not set");

x.clear();
if x.testAndSet() then
  writeln("set");
 else
   writeln("not set");

on Locales[numLocales-1] {
  if x.testAndSet() then
    writeln("set");
  else
    writeln("not set");
}
