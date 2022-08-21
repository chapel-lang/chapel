
use IO;

record R {
  var x : int;
}

operator R.<~>(ref r : R, arg : int) ref {
  r.x = arg;
  return r;
}

proc main() {
  var f = openmem();
  {
    var w = f.writer();
    w <~> "hello";
    w <~> new ioNewline();
    w <~> new ioLiteral("-----\n");
    w <~> "goodbye";
  }
  {
    var r = f.reader();
    var s : string;
    r <~> s;
    writeln(s);
    r <~> new ioNewline();
    r <~> new ioLiteral("-----\n");
    r <~> s;
    writeln(s);
  }
  {
    var r = new R(0);
    r <~> 5 <~> 10 <~> 42;
    writeln(r);
  }

}
