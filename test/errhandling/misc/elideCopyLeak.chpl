
use Map, IO;

proc readKey(r, type keyType) : keyType throws {
  r.matchLiteral(",");
  return r.read(keyType);
}

proc readVal(r, type valType) : valType throws {
  r.readLiteral(":");
  return r.read(valType);
}

proc ref map.readMe(r) throws {
  while !r.matchLiteral("}") {
    // This .future requires further investigation, but the problem seems to
    // be that the compiler never inserts an auto-destroy for the results of
    // 'readKey' and 'readVal', and so they never get cleaned up when the
    // error is thrown.
    add(readKey(r, keyType), readVal(r, valType));
  }
}

proc main() {
  var f = openMemFile();
  f.writer(locking=false).write("{a: b, c: d, e: f}");

  var m : map(string, string);
  var r = f.reader(locking=false);
  r.readLiteral("{");
  try {
    m.readMe(r);
  } catch {
    writeln("ERROR");
  }
}
