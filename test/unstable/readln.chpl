use IO;

var i: int, r: real;

writeln(readln(int, real));
readln(i, r); writeln((i, r));

var fr = openReader("readln.stdin", locking=false);
fr.readln(i, r); writeln((i, r));
writeln(fr.readln(int, real));
