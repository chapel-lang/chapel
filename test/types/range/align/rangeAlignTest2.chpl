var r1: range(int);
writeln(r1.isAligned());

var r2: range(int, strides=strideKind.any);
writeln(r2.isAligned());
