var r:range(strides=strideKind.one);
writeln(r);

proc test(arg:range(strides=strideKind.any)) {
  writeln(arg);
}

test(r);
