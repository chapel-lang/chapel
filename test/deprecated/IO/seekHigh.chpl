use IO;

var ch = openreader("seek.txt", locking=false);
ch.seek(3..8);
writeln(ch.readLine());
