
use IO;

record G {
  var x;
}

record I {
  var x : integral;
}

config type T = G;

proc main() {
  var f = openmem();
  {
    f.writer().write(new T(5));
  }
  {
    var val = f.reader().withFormatter(DefaultReader).read(T(int));
    writeln(val);
  }
}
