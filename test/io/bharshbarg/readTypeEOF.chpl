
use IO;

// This test exists to ensure that the ``channel.read*`` methods that accept
// arguments will throw an EOF error.

proc main() {
  var f = openmem();
  var r = f.reader();

  try {
    var x = r.read(int);
    writeln(x);
    writeln("ERROR: expected EOF");
  } catch e : Error {
    writeln(e.message());
  }

  try {
    var x = r.readln(int);
    writeln(x);
    writeln("ERROR: expected EOF");
  } catch e : Error {
    writeln(e.message());
  }

  try {
    var (x, y, z) = r.read(int, real, bool);
    writeln(x, " :: ", y, " :: ", z);
    writeln("ERROR: expected EOF");
  } catch e : Error {
    writeln(e.message());
  }

  try {
    var (x, y, z) = r.readln(int, real, bool);
    writeln(x, " :: ", y, " :: ", z);
    writeln("ERROR: expected EOF");
  } catch e : Error {
    writeln(e.message());
  }
}
