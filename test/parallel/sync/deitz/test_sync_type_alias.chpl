proc main {
  type t = sync int;
  var v: t;
  v = 1;
  writeln(v.readFE());
}
