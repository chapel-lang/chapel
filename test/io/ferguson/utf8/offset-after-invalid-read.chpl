use IO, CTypes;

// This test checks that invalid UTF-8 errors from reading a
// character-at-a-time leave the channel in the expected position.
// At the time of this writing, that is at the first byte
// that is problematic.

config const verbose = false;

extern var qbytes_iobuf_size:c_size_t;

var defaultBufSz = qbytes_iobuf_size: int ;

proc testWithBuf(invalid: bytes, expectOffset: int, bufsz: int = defaultBufSz) {
  qbytes_iobuf_size = bufsz: c_size_t;

  var f = openMemFile();
  f.writer(locking=false).write(b"abc" + invalid + b"xyz");

  var r = f.reader(locking=false);
  try {
    var codepoint: int;
    while true {
      if verbose then 
        writef("offset %i ", r.offset());
      r.readCodepoint(codepoint);
      if verbose then
        writef("codepoint %c (%xi)\n", codepoint, codepoint);
    }
  } catch e {
    if verbose {
      writeln("caught expected error ", e);
      writeln("reader position is ", r.offset());
    }
    assert(r.offset() == expectOffset + 3); // 3 to pass "abc"
  }
}

proc test(invalid: bytes, expectOffset: int) {
  writeln("testing ", invalid.toHexadecimal(uppercase=true));
  testWithBuf(invalid, expectOffset, defaultBufSz);
  for buffsz in 1..10 {
    testWithBuf(invalid, expectOffset, buffsz);
  }
  qbytes_iobuf_size = defaultBufSz: c_size_t;
}

// this is the byte patterns for valid UTF-8
// 1 byte: 0xxxxxxx
// 2 byte: 110xxxxx 10xxxxxx
// 3 byte: 1110xxxx 10xxxxxx 10xxxxxx
// 4 byte: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx

// testing invalid UTF-8 patterns

// 1 byte:
//   11111111 -- FF -- not a legal start byte
test(b"\xFF", 1);
//   10001000 -- 88 -- unexpected continuation byte
test(b"\x88", 1);

// 2 byte:
//   11000010 00100100 -- C2 24 -- (missing continuation)
test(b"\xC2\x24", 2);

// 3 byte:
//   11100000 00100100 00100100 -- E0 24 24 -- (missing continuation)
test(b"\xE0\x24\x24", 2);
//   11100000 10100100 00100100 -- E0 A4 24 -- (missing continuation)
test(b"\xE0\xA4\x24", 3);

// 4 byte: (missing continuation)
//   11110000 00100100 00100100 00100100 -- F0 24 24 24
test(b"\xF0\x24\x24\x24", 2);
//   11110000 10010000 00100100 00100100 -- F0 90 24 24
test(b"\xF0\x90\x24\x24", 3);
//   11110000 10010000 10001101 00100100 -- F0 90 8D 24
test(b"\xF0\x90\x8D\x24", 4);
