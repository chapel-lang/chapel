use Random;

proc main() throws {

  var pcgStream = PCGRandomStream.create(int);
  // Note: getNext(min, max) is not available for NPB

  var x = pcgStream.getNext(2, 1);
  writeln('Empty range of values did not halt');
}
