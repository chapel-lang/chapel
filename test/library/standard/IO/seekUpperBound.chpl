use IO;

var ch = openReader("seek.txt", locking=false);
ch.seek(3..8);
writeln(ch.readLine());  // Should include the space after "mi"
