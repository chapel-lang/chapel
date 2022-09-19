
use IO;
use OS;

//
// Test cases:
// - successfully read a literal
// - fail to read a literal
//   - due to format error
//   - due to EOF
// - read a literal on a json channel
// - ignoreWhitespace
// - speculative read
// - readNewline
//
// for both bytes and string types
//

config param useStrings = true;

var fiveSquare = if useStrings then "[5]" else b"[5]";
var LBR = if useStrings then "[" else b"[";
var RBR = if useStrings then "]" else b"]";

proc testBasic() throws {
  var f = openmem();
  {
    var w = f.writer();
    w.write(fiveSquare);
  }
  var r = f.reader();
  r.readLiteral(LBR);
  writeln(r.read(int));
  r.readLiteral(RBR);
  assert(r.atEOF());
}

proc testFailure() {
  var f = openmem();
  {
    var w = f.writer();
    w.write(fiveSquare);
  }

  var r = f.reader();
  try {
    param paren = if useStrings then "(" else b"(";
    r.readLiteral(paren); // trying to read the wrong opening symbol
    assert(false);
  } catch e:BadFormatError {
    writeln("caught expected format error");
    assert(r.offset() == 0);
  } catch {
    assert(false);
  }

  r.readLiteral(fiveSquare);
  try {
    param foo = if useStrings then "foo" else b"foo";
    r.readLiteral(foo);
  } catch e:EofError {
    writeln("caught expected EOF error");
    assert(r.atEOF());
  } catch {
    assert(false);
  }
}

proc testJSON() {
  var f = openmem();
  param quote = if useStrings then '"' else b"\"";
  {
    var w = f.writer();
    w.write(quote);
    w.write(fiveSquare);
    w.write(quote);
  }

  var r = f.reader(locking=false);
  var st = r._styleInternal();
  st.string_format = iostringformat.json:uint(8);
  r._set_styleInternal(st);

  try {
    var x = "[";
    r.mark();
    r.read(x);
    const off = r.offset();
    assert(r.atEOF());
    r.revert();
    if off != 1 then throw new BadFormatError("msg");
  } catch e:BadFormatError {
    writeln("caught expected format error with JSON");
  } catch {
    assert(false);
  }

  r.readLiteral(quote + LBR);
  writeln(r.read(int));
  r.readLiteral(RBR + quote);
  assert(r.atEOF());
}

proc testWhitespace() {
  var f = openmem();
  {
    var w = f.writer();
    const str = "   [5   ]";
    const spaced = if useStrings then str else str.encode();
    w.write(spaced);
  }

  var r = f.reader(locking=false);
  r.mark();
  r.readLiteral(LBR);
  writeln(r.read(int));
  r.readLiteral(RBR);
  assert(r.atEOF());
  r.revert();

  try {
    r.readLiteral(LBR, ignoreWhitespace=false);
  } catch e:BadFormatError {
    writeln("caught expected format error with whitespace");
  } catch {
    assert(false);
  }
}

proc testSpeculative() {
  param comma = if useStrings then "," else b",";
  var f = openmem();
  {
    var w = f.writer();
    w.write(LBR);
    for i in 1..9 do w.write(i, comma);
    w.write(10);
    w.write(RBR);
  }

  var A : [1..10] bool;
  var r = f.reader();
  r.readLiteral(LBR);
  while true {
    const i = r.read(int);
    A[i] = true;

    try {
      r.readLiteral(comma);
    } catch e:BadFormatError {
      writeln("caught comma");
      break;
    } catch {
      assert(false);
    }
  }
  r.readLiteral(RBR);
  assert(r.atEOF());
  assert(&& reduce A);
}

proc testNewline() {
  var f = openmem();
  const numSpaces = 5;
  const start = "start";
  {
    var w = f.writer();
    w.write(start);
    // by default readnewline should read past contiguous whitespace
    w.write(" " * numSpaces);
    for 1..10 do w.writeln();
    w.write("end");
  }
  {
    // try to read a newline with 'start' in the way
    var r = f.reader();
    try {
      r.readNewline();
    } catch e:BadFormatError {
      writeln("caught newline trying to read non-whitespace");
    } catch {
      assert(false);
    }
  }
  {
    var r = f.reader();
    r.readLiteral(start);
    r.readNewline();
    assert(r.offset() == start.size + numSpaces + 1);
  }
  {
    var r = f.reader();
    r.readLiteral(start);
    while true {
      try {
        r.readNewline();
      } catch e:BadFormatError {
        writeln("caught expected format error");
        break;
      } catch {
        assert(false);
      }
    }
    var x = r.read(string);
    assert(x == "end");
    assert(r.atEOF());
    try {
      r.readNewline();
    } catch e:EofError {
      writeln("caught expected eof error");
    } catch {
      assert(false);
    }
  }
}

proc main() {
  testBasic();
  testFailure();
  testJSON();
  testWhitespace();
  testSpeculative();
  testNewline();

  writeln("SUCCESS");
}
