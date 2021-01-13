config var n1 = 1;
config var n2 = 4;
config var n3 = 2;
// String
writeln("Chapel"[n1]);
writeln("Chapel"[n1..n2]);
writeln("Chapel"[n1..]);
writeln("Chapel"[..n3]);
writeln("Chapel"[n1:byteIndex..n3:byteIndex]);

// Bytes
writeln(b"Chapel"[n1]);
writeln(b"Chapel"[n1..n2]);
writeln(b"Chapel"[n1..]);
writeln(b"Chapel"[..n3]);
writeln(b"Chapel"[n1:byteIndex..n3:byteIndex]);
