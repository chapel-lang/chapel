use IO, CTypes;
var f = stdout;
f.writeln("hello");
again(f);
proc again(const in g: fileWriter(true)) {
  if g == stdout {
    writeln("hello again");
  }
  if g != stdout {
  } else {
    writeln("and again");
  }
}
