use IO;

// string
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=5));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=6));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=7));

writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=5, true));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=6, true));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=7, true));

// bytes
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=5));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=6));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=7));

writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=5, true));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=6, true));
writeln(openreader("maxSizeInput.txt").readThrough(b"x", maxSize=7, true));

// confirm the pointer gets left off after 'maxSize' bytes/codepoints
var r = openreader("maxSizeInput.txt");
writeln(r.readThrough("x", maxSize=5));
write(r.readAll());
r.close();

r = openreader("maxSizeInput.txt");
writeln(r.readThrough(b"x", maxSize=5));
write(r.readAll());
