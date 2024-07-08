use Random;

proc main() throws {
  var stream = new randomStream(int);

  var x = stream.next(2, 1);
  writeln('Empty range of values did not halt');
}
