
use IO;
use PrecisionSerializer;
use FormatHelper;

proc makeND(param rank : int) {
  var dims : rank*range;
  for d in dims do d = 1..4;
  var dom = {(...dims)};
  var A : [dom] real;
  for i in 0..<A.size {
    A[dom.orderToIndex(i)] = i:real + 0.123456789;
  }
  return A;
}

proc test(A, ser) {
  writeln("===== writing: =====");
  stdout.writeln(A);
  writeln("--------------------");
  stdout.withSerializer(ser).writeln(A);

  var f = openMemFile();
  try {
    f.writer().withSerializer(ser).write(A);
  } catch e {
    writeln("ERROR: ", e);
  }

  try {
    var B = f.reader().read(A.type);

    var match = true;
    for (a, b) in zip(A, B) do
      match &&= (abs(a - b) < 0.001);

    if !match {
      writeln("FAILURE: ", A.type:string);
      writeln("GOT: ", B);
    } else
      writeln("SUCCESS: ", A.type:string);

  } catch e {
    writeln("ERROR: ", e);
  }
}

proc main() {
  for (pre, pad) in [(8, 0), (4, 12)] {
    writeln("##### precision=", pre, ", padding=", pad, " ######");
    for param i in 1..4 {
      writeln("----- ", i:string, "D -----");
      var A = makeND(i);
      test(A, new precisionSerializer(pre, pad));
    }
  }
}
