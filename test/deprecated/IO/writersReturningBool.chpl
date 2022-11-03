use IO;
use IO.FormattedIO;
import FileSystem.remove;

var w = openwriter("./wrb.txt");

w.write("a");
w.writeln("b");

w.writebits(097, 8);
w.writeln();

w.writeBytes(098, 1);
w.writeln();

w.writef("%i", 5);
w.writeln();

writef("%i\n", 5);
