var ax: atomic bool;
if ax.testAndSet() then
  writeln("set");
 else
   writeln("not set");
if ax.testAndSet() then
  writeln("set");
 else
   writeln("not set");

ax.clear();
if ax.testAndSet() then
  writeln("set");
 else
   writeln("not set");

on Locales[numLocales-1] {
  if ax.testAndSet() then
    writeln("set");
  else
    writeln("not set");
}

var x = ax.read();
writeln(x);

ax.write(false);
writeln(ax);
