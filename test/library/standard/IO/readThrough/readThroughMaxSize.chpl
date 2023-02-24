use IO;

// string
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=5));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=6));
writeln(openreader("maxSizeInput.txt").readThrough("x", maxSize=8));

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
