
use IO;
use FormatHelper;
use BlockDist;

proc main() {
  var D = {1..4, 1..4, 1..4};
  var Space = D dmapped new blockDist(D);
  var B : [Space] int;

  var cur = 0;
  for b in B {
    b = cur;
    cur += 1;
  }

  var f = openMemFile();
  {
    f.writer(serializer=FormatWriter, locking=false).writeln(B);
  }
  {
    var DR = f.reader(deserializer=FormatReader, locking=false).read([D] int);
    printDebugFmt(DR);

    assert(&& reduce (B == DR));
  }
}
