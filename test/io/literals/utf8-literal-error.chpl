
use IO;

//
// Make sure we print something a bit more useful for invalid bytes
//
// The SystemError text differs between mac and linux:
// mac: Illegal byte sequence
// linux: Invalid or incomplete multibyte or wide character
//

proc main() {
  var b = b"\x80\x81";
  var f = openMemFile();
  {
    var w = f.writer();
    w.write("hello");
  }
  var r = f.reader();

  try {
    r.readLiteral(b);
    assert(false);
  } catch e : SystemError {
    assert(e.message().count("non-UTF8 bytes") > 0);
  } catch {
    assert(false);
  }

  try {
    r.matchLiteral(b);
    assert(false);
  } catch e : SystemError {
    assert(e.message().count("non-UTF8 bytes") > 0);
  } catch {
    assert(false);
  }
}
