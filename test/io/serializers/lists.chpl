
use IO;
use JSON;
use FormatHelper;
use List;

proc test(A) {
  writeln("--- ", A.type:string, " ---");
  printDebugFmt(A);
  var f = openMemFile();
  try {
    f.writer(locking=false).withSerializer(FormatWriter).write(A);
  } catch e {
    writeln("ERROR: ", e);
  }
  try {
    var B = f.reader(locking=false).withDeserializer(FormatReader).read(A.type);

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
    for i in 1..10 do li.pushBack(i**2);
    test(li);
  }
  {
    var li : list(int);
    test(li);
  }
  {
    var li : list(R);
    for i in 1..4 do li.pushBack(new R(i**2));
    test(li);
  }
  {
    var li : list(list(int));
    for i in 1..4 {
      var cur : list(int);
      for j in (i-1)*4..#3 do cur.pushBack(j);
      li.pushBack(cur);
    }
    test(li);
  }
}
