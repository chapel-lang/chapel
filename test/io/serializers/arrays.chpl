
use IO;
use Json;
use FormatHelper;
use List;

proc makeND(param rank : int) {
  var dims : rank*range;
  for d in dims do d = 1..3;
  var dom = {(...dims)};
  var A : [dom] int;
  var count = 0;
  for a in A {
    a = count;
    count += 1;
  }
  return A;
}

proc test(A) {
  printDebugFmt(A);
  var f = openMemFile();
  try {
    f.writer().withSerializer(FormatWriter).write(A);
  } catch e {
    writeln("ERROR: ", e);
  }
  try {
    var B = f.reader().withDeserializer(FormatReader).read(A.type);

    var match = false;
    if isArray(A) {
      if isArrayType(A.eltType) {
        match = true;
        for (x,y) in zip(A,B) do
          for (a,b) in zip(x,y) do
            match &&= (a == b);
      } else {
        match = && reduce (A == B);
      }
    } else {
      match = (A == B);
    }

    if !match {
      writeln("FAILURE: ", A.type:string);
      writeln("GOT: ", B);
    } else
      writeln("SUCCESS: ", A.type:string);
  } catch e {
    writeln("ERROR: ", e);
  }
}

record R {
  var x : [1..4] int = 1..4;
}

// TODO: sparse?
proc main() {
  for param i in 1..5 {
    writeln("----- ", i:string, "D -----");
    var A = makeND(i);
    test(A);
  }
  {
    writeln("--- array of arrays ---");
    var A : [1..3] [1..3] int;
    for i in 1..3 do
      for j in 1..3 do
        A[i][j] = i*j;
    test(A);
  }
  {
    writeln("--- record with array field ---");
    var r = new R();
    test(r);
  }
}
