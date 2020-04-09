var global = 10;
const ref locco = global;
ref       locre = global;

proc main() {
  cobegin with (ref global) {
    {
      writeln(locco);
      writeln(locre);
      global = 20;
      writeln(locco);
      writeln(locre);
    }
    {
      var jjj: int;
    }
  }
  writeln();
  sync {
    begin with (ref global) {
      writeln(locco);
      writeln(locre);
      global = 30;
      writeln(locco);
      writeln(locre);
    }
  }
}
