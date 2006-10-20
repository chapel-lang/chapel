use Memory;
use Types;

module HPCCProblemSize {
  def computeProblemSize(type elemType, numArrays, returnPow2 = false,
                         memRatio:int = 4) {
    const totalMem = + reduce physicalMemory(Locale, unit = Bytes),
          memoryTarget = totalMem / memRatio,
          bytesPerIndex = numArrays * numBytes(elemType);

    var numIndices = memoryTarget / bytesPerIndex;

    if (returnPow2) {
      var lgProblemSize = lg(numIndices);
      numIndices = 0x1 << lgProblemSize;  // BLC: exponentiation
      if (numIndices * bytesPerIndex <= memoryTarget) {
        numIndices *= 2;
      }
    }

    const smallestMem = min reduce physicalMemory(Locale, unit = Bytes);
    if ((numIndices * bytesPerIndex)/numLocales > smallestMem) then
      halt("System is too heterogeneous: blocked data won't fit into memory");

    return numIndices;
  }

  def printProblemSize(type elemType, numArrays, problemSize: ?psType) {
    const bytesPerArray = problemSize * numBytes(elemType): psType,
          totalMemInGB = (numArrays * bytesPerArray:float) / (1024**3),
          lgProbSize = lg(problemSize):psType;
    write("Problem size = ", problemSize);
    if (2**lgProbSize == problemSize) {
      write(" (2**", lgProbSize, ")");
    }
    writeln();
    writeln("Bytes per array = ", bytesPerArray);
    writeln("Total memory required (GB) = ", totalMemInGB);
  }
}
