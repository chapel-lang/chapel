use IO, CTypes;
var f = stdin;
writeln("hello");
on Locales.last do again(f);
proc again(const in g: fileReader(true)) {
  if g == stdin {
    writeln("hello again");
  }
  if g != stdin {
  } else {
    writeln("and again");
  }
}
