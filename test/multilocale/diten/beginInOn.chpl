use Time;

def main {
  var a: int;
  on Locales(1) {
    begin {
      a = 1;
    }
  }
  while (a == 0) { /* spin! */ }
  if (a != 1) {
    writeln("variable is bad: ", a);
  }
}
