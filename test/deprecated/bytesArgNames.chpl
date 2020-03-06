var s = b"test";
var c = c"test";
var cp = c_malloc(uint(8), 3);

writeln(createBytesWithBorrowedBuffer(s=s));
writeln(createBytesWithBorrowedBuffer(s=c));
writeln(createBytesWithBorrowedBuffer(s=cp, 2, 3));

// writeln(createBytesWithOwnedBuffer(s=s)); -- this one's an error already
writeln(createBytesWithOwnedBuffer(s=c));
writeln(createBytesWithOwnedBuffer(s=cp, 2, 3));

writeln(createBytesWithNewBuffer(s=s));
writeln(createBytesWithNewBuffer(s=c));
writeln(createBytesWithNewBuffer(s=cp, 2, 3));

var st = (b"s1", b"s2", b"s3");
var sa = (b"s1", b"s2", b"s3");

writeln(b",".join(S=st));
writeln(b",".join(S=sa));
