
use IO;

// This test is not intended to be executed, only compiled for the sake of
// deprecation warnings.

proc main() {
  var st : iostyle;
  var m = openmem();

  { var f = open("foo.txt", iomode.r, hints=IOHINT_NONE, st); }
  { var f = openfd(0, hints=IOHINT_NONE, st); }
  { var _tmp : _file;
    var f = openfp(_tmp, hints=IOHINT_NONE, st); }
  { var f = opentmp(hints=IOHINT_NONE, st); }

  { var r = openreader("foo.txt", hints=IOHINT_NONE, style=st); }
  { var w = openwriter("foo.txt", hints=IOHINT_NONE, style=st); }

  { var r = m.reader(hints=IOHINT_NONE, style=st); }
  { var w = m.writer(hints=IOHINT_NONE, style=st); }
  { var L = m.lines(hints=IOHINT_NONE, local_style=st); }
}
