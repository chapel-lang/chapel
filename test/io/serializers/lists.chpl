
use IO;
use Json;
use FormatHelper;
use List;

proc test(A) {
  writeln("--- ", A.type:string, " ---");
  printDebugFmt(A);
  var f = openTempFile();
  try {
    f.writer().withSerializer(FormatWriter).write(A);
  } catch e {
    writeln("ERROR: ", e);
  }
  try {
    var B = f.reader().withDeserializer(FormatReader).read(A.type);

    var match = false;
    if isArray(A) {
      match = && reduce (A == B);
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
  var x : int;
}

proc main() {
  {
    var li : list(int);
    for i in 1..10 do li.append(i**2);
    test(li);
  }
  {
    var li : list(int);
    test(li);
  }
  {
    var li : list(R);
    for i in 1..4 do li.append(new R(i**2));
    test(li);
  }
  {
    var li : list(list(int));
    for i in 1..4 {
      var cur : list(int);
      for j in (i-1)*4..#3 do cur.append(j);
      li.append(cur);
    }
    test(li);
  }
}
