module HPCCProblemSize {
  use MemDiagnostics, Types;

  config const memRatio = 4;

  proc computeProblemSize(type elemType, numArrays, returnLog2 = false) {

    //
    // Compute the total memory available to the benchmark. If there is one
    // locale per node, then compute the total using a sum reduction over the
    // amount of physical memory (in bytes) owned by the set of locales on
    // which we're running. Otherwise, sum the physical memory of unique
    // nodes as determined by each locale's hostname. Then compute the number
    // of bytes each locale will use as defined by memFraction and the
    // maximum number of co-locales on any node, and the size of each index.
    //

    var totalMem = 0;
    if (max reduce Locales.numColocales > 1) {
      var nodes: domain(string, parSafe=false);
      for loc in Locales {
        if (nodes.contains(loc.hostname) == false) {
          nodes += loc.hostname;
          totalMem += loc.physicalMemory(unit = MemUnits.Bytes);
        }
      }
    } else {
      totalMem = + reduce Locales.physicalMemory(unit = MemUnits.Bytes);
    }

    const memoryTarget = totalMem / memRatio,
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
    const bytesPerArray = problemSize * numBytes(elemType),
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
