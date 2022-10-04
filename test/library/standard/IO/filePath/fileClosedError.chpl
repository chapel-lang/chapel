use IO;

var f = open("./test.txt", iomode.r);
f.close();

try {
    var p = f.path;
} catch e {
    writeln(e);
}
