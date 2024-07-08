use IO;

var fm = openMemFile();
try {
    // path doesn't work on openMemFile files, this call should throw
    var p = fm.path;
} catch e {
    writeln(e);
}
