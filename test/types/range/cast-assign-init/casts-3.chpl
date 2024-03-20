
var r = 1..10 by 1000;

type types = (bool, int, uint, int(8), uint(8));

for param i in 0..<types.size {
  if types(i)!=bool then  // tryCast() into range(bool) is disabled, see #22905
  try {
    writeln(r.tryCast(range(types(i), strides=strideKind.any)));
  } catch e {
    writeln(e);
  }
}

writeln();

for param i in 0..<types.size {
    writeln(r: range(types(i), strides=strideKind.any));
}
