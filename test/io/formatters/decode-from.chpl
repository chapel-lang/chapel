
use IO;

record R {
  var x : int;

  proc encodeTo(w: fileWriter) {
    w.write(x);
  }

  proc type decodeFrom(r: fileReader) {
    return new R(r.read(int));
  }
}

record G {
  var x;

  proc encodeTo(w: fileWriter) {
    w.write(x);
  }

  proc type decodeFrom(r: fileReader) {
    type fieldType = __primitive("field by num", this, 1);
    return new G(r.read(fieldType));
  }
}

proc test(val, type T) {
  var f = openMemFile();
  {
    f.writer().write(val);
  }
  {
    var read = f.reader().read(T);
    assert(read == val);
  }
}

proc main() {
  test(new R(5), R);
  test(new G(42.0), G(real));
  writeln("SUCCESS");
}
