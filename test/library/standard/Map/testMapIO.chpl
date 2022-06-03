
use IO;
use Map;

proc main() {
  var f = openmem();

  var m = new map(int, real);
  for i in 1..10 do m[i] = (i**2):real;

  var w = f.writer();
  w.write(m);
  w.close();

  {
    var r = f.reader();
    var contents : string;
    r.readstring(contents);
    writeln("Wrote:");
    writeln("==========");
    writeln(contents.strip());
    writeln("==========");
  }

  var r = f.reader();
  var x : map(int,real);
  try {
    r.read(x);
    writeln(x);
  } catch (e : Error) {
    writeln("Error reading map:");
    writeln(e.message());
  }

}
