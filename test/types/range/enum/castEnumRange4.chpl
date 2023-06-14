enum color {red=42, orange=33};
var r1 = color.red..color.orange;
var r2 = r1: range(color, boundKind.both, strideKind.any);
writeln(r2, (r2.low, r2.high, r2.stride, r2.alignment));
