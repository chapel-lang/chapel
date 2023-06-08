var r1: range(int);
writeln(r1.aligned);

var r2: range(int, strides=strideKind.any);
writeln(r2.aligned);
