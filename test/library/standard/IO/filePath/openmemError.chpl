use IO;

var fm = openmem();
try {
    // path doesn't work on openmem files, this call should throw
    var p = fm.path;
} catch e {
    writeln(e);
}
