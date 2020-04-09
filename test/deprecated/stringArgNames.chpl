var s = "test";
var c = c"test";
var cp = c_malloc(uint(8), 3);

writeln(createStringWithBorrowedBuffer(s=s));
writeln(createStringWithBorrowedBuffer(s=c));
writeln(createStringWithBorrowedBuffer(s=cp, 2, 3));

// writeln(createStringWithOwnedBuffer(s=s)); -- this one's an error already
writeln(createStringWithOwnedBuffer(s=c));
writeln(createStringWithOwnedBuffer(s=cp, 2, 3));

writeln(createStringWithNewBuffer(s=s));
writeln(createStringWithNewBuffer(s=c));
writeln(createStringWithNewBuffer(s=cp, 2, 3));

var st = ("s1", "s2", "s3");
var sa = ("s1", "s2", "s3");

writeln(",".join(S=st));
writeln(",".join(S=sa));
