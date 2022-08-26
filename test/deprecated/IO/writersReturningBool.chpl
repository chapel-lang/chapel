use IO;
use IO.FormattedIO;
import FileSystem.remove;

var w = openwriter("./wrb.txt");

w.write("a");
w.writeln("b");
w.writebits(2, 2);
w.writeBytes(0xF, 1);
w.writef("%i", 5);

w.close(); remove("./wrb.txt");

writef("%i\n", 5);
