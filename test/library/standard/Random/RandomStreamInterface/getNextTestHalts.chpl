use Random;

proc main() throws {

  var pcgStream = createRandomStream(int, algorithm=RNG.PCG);
  // Note: getNext(min, max) is not available for NPB

  var x = pcgStream.getNext(2, 1);
  writeln('Empty range of values did not halt');
}
