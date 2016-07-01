var indices: domain(int);
var elems  : [indices] real;

const retval = indices.remove(5);

writeln("indices is: ", indices);
writeln("elems is: ", elems);
writeln("# ind removed: ", retval);
