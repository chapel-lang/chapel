use IO, CTypes;
var f = stderr;
writeln("hello");
on Locales.last do again(f);
proc again(const in g: fileWriter(true)) {
  if g == stderr {
    writeln("hello again");
  }
  if g != stderr {
  } else {
    writeln("and again");
  }
}
