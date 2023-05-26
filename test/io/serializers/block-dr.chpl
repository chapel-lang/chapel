
use IO;
use FormatHelper;
use BlockDist;

proc main() {
  var D = {1..4, 1..4, 1..4};
  var Space = D dmapped Block(D);
  var B : [Space] int;

  var cur = 0;
  for b in B {
    b = cur;
    cur += 1;
  }

  var f = openTempFile();
  {
    f.writer(serializer=FormatWriter).writeln(B);
  }
  {
    var DR = f.reader(deserializer=FormatReader).read([D] int);
    printDebugFmt(DR);

    assert(&& reduce (B == DR));
  }
}
