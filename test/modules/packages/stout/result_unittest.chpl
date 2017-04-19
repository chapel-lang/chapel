use stout;

proc main() {
  var s = Result(int).some(0);
  writeln(s.isSome());
  writeln(s.get());
  writeln(s());
  var n = Result(int).none(int);
  writeln(n.isNone());
  var e = Result(string).error(string, "string");
  writeln(e.error());
  writeln(e.isError());
}
