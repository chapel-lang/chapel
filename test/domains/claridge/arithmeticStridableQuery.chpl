var D_unstrided: domain(1, strides=strideKind.one);
writeln(D_unstrided.strides);

var D_strided: domain(1, strides=strideKind.any);
writeln(D_strided.strides);