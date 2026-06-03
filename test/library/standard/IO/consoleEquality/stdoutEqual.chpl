use IO, CTypes;
var f = stdout;
writeln("hello");
on Locales.last do again(f);
proc again(const in g: fileWriter(true)) {
  if g == stdout {
    writeln("hello again");
  }
  if g != stdout {
  } else {
    writeln("and again");
  }
}
