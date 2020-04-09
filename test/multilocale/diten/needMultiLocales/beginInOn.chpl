use Time;

proc main {
  var a: atomic int;
  on Locales(1) {
    begin {
      a.write(1);
    }
  }
  while (a.read() == 0) { /* spin! */ }
  if (a.read() != 1) {
    writeln("variable is bad: ", a.read());
  }
}
