use IO;

var f = open("./test.txt", ioMode.r);
f.close();

try {
    // The file is closed, this call should throw
    var p = f.path;
} catch e {
    writeln(e);
}
