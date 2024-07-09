var a:range(int, strides=strideKind.one) = 1..10;
var b = a:range(int, strides=strideKind.any);
writeln(a);
writeln(b);
