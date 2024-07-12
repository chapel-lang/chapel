record R {
  proc deinit() do writeln("deinit");
}
class C {
  proc deinit() do writeln("deinit");
}

config type T = R;

proc get() throws {
  var a = new T();
  return a;
}

proc main() {
  var a = try! get();
  try! get();
}
