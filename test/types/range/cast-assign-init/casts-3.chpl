
var r = 1..10 by 1000;

type types = (int, uint, int(8), uint(8), bool);

for param i in 0..<types.size {
  try {
    writeln(r: range(types(i), strides=strideKind.any));
  } catch e {
    writeln(e);
  }
}
