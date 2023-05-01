module HPCCProblemSize {
  use MemDiagnostics;
  use Types;

  config const memRatio = 4;

  proc computeProblemSize(type elemType, numArrays, returnLog2 = false) {
    const totalMem = + reduce Locales.physicalMemory(unit = MemUnits.Bytes),
          memoryTarget = totalMem / memRatio,
          bytesPerIndex = numArrays * numBytes(elemType);

    var numIndices = (memoryTarget / bytesPerIndex): int;

    var lgProblemSize = log2(numIndices);
    if (returnLog2) {
      numIndices = 2**lgProblemSize;
      if (numIndices * bytesPerIndex <= memoryTarget) {
        numIndices *= 2;
        lgProblemSize += 1;
      }
    }

    const smallestMem = min reduce Locales.physicalMemory(unit = MemUnits.Bytes);
    if ((numIndices * bytesPerIndex)/numLocales > smallestMem) then
      halt("System is too heterogeneous: blocked data won't fit into memory");

    return if returnLog2 then lgProblemSize else numIndices;
  }


  proc printProblemSize(type elemType, numArrays, problemSize: ?psType) {
    const bytesPerArray = problemSize * numBytes(elemType): psType,
          totalMemInGB = (numArrays * bytesPerArray:real) / (1024**3),
          lgProbSize = log2(problemSize):psType;

    write("Problem size = ", problemSize);
    if (2**lgProbSize == problemSize) {
      write(" (2**", lgProbSize, ")");
    }
    writeln();
    writeln("Bytes per array = ", bytesPerArray);
    writeln("Total memory required (GB) = ", totalMemInGB);
  }
}
