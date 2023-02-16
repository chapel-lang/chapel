
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

class C {
  var x : int;

  proc encodeTo(w: fileWriter) {
    w.writeLiteral("<");
    w.write(x);
    w.writeLiteral(">");
  }

  proc type decodeFrom(r: fileReader) {
    type retType = this;
    r.readLiteral("<");
    var ret = new retType(r.read(int));
    r.readLiteral(">");
    return ret;
  }

  proc equals(other: borrowed C) {
    return this.x == other.x;
  }
}

proc test(val, type T) {
  var f = openMemFile();
  {
    f.writer().write(val);
  }
  {
    var read = f.reader().read(T);

    if isClassType(T) {
      if isNilableClassType(T) {
        if val == nil then
          assert(read == nil);
        else assert(val!.equals(read!));
      } else {
        assert(val.equals(read));
      }
    } else {
      assert(read == val);
    }
  }
}

proc main() {
  test(new R(5), R);
  test(new G(42.0), G(real));

  // test 'nil' case to make sure formatter can still control the reading of
  // the 'nil' string (as compared to something like "null" for json).
  test(nil, unmanaged C?);
  test(new C(5), owned C);
  writeln("SUCCESS");
}
