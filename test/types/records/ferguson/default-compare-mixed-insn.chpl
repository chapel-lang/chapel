record R {
  type t = int;
  var x:t;
}

proc main() {
  var r64 = new R(int(64), 1);
  var r64a = new R(int(64), 1);
  var r32 = new R(int(32), 1);
  var r32a = new R(int(32), 1);

  writeln(r64 == r64a);
  writeln(r32 == r32a);

  // These return false because the default == fn
  // compares the type fields and they differ.
  writeln(r32 == r64);
  writeln(r64 == r32);
}
