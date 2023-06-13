const D1: domain(1, strides=strideKind.any) = {1..10 by -2};

writeln(D1 , " yields: ");
for i in D1 do
  writeln(i);
