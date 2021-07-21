use IO, Path;

var f: file;
// File records are initialised to not point to a valid file pointer.
var fPath = realPath(f);
writeln(fPath); // Shouldn't reach here
