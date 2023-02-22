use IO;

var f = openTempFile();

// not checking OS-specific implementations for now...
try {
    var p = f.path;
    writeln("didn't throw");
} catch {
    writeln("threw!");
}
