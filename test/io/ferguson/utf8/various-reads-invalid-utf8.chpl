// this test locks in the behavior for various I/O functions
// when encountering invalid UTF-8.

use IO;

config const verbose = false;

proc makeReader() {
  var f = openMemFile();
  f.writer(locking=false).write(b"abc\xE0\x24\x24xyz\n");
  return f.reader(locking=false);
}

proc expect(r, e, expectedOffset) {
  if verbose {
    writeln("caught expected error ", e);
    writeln("channel offset ", r.offset(), " expected ", expectedOffset);
  }
  assert(r.offset() == expectedOffset);
  writeln("  got expected error and offset");
}

proc unexpected() {
  writeln("  missing error");
}

proc expectNoError() {
  writeln("  no error, as expected");
}

proc main() {
  {
    writeln("readLine");
    var r = makeReader();
    try {
      r.readLine();
      unexpected();
    } catch e {
      expect(r, e, 0);
    }
  }

  {
    writeln("readThrough");
    var r = makeReader();
    try {
      var s: string;
      r.readThrough("x", s);
      unexpected();
    } catch e {
      expect(r, e, 0);
    }
  }

  {
    writeln("readTo");
    var r = makeReader();
    try {
      var s: string;
      r.readTo("x", s);
      unexpected();
    } catch e {
      expect(r, e, 0);
    }
  }

  {
    writeln("advanceThrough");
    var r = makeReader();
    try {
      r.advanceThrough("x");
      expectNoError();
    } catch e {
      expect(r, e, 0);
    }
  }

  {
    writeln("advanceTo");
    var r = makeReader();
    try {
      r.advanceTo("x");
      expectNoError();
    } catch e {
      expect(r, e, 0);
    }
  }

  {
    writeln("readString");
    var r = makeReader();
    try {
      var s: string;
      r.readString(100);
      unexpected();
    } catch e {
      expect(r, e, 5);
    }
  }

  {
    writeln("readAll(string)");
    var r = makeReader();
    try {
      var s: string = r.readAll(string);
      unexpected();
    } catch e {
      expect(r, e, 5);
    }
  }

  {
    writeln("readBinary(string)");
    var r = makeReader();
    try {
      var s: string;
      r.readBinary(s, 100);
      unexpected();
    } catch e {
      expect(r, e, 5);
    }
  }
}
