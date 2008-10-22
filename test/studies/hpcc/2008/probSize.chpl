//
// A shared module for computing the appropriate problem size for the
// HPCC benchmarks
//
module HPCCProblemSize {
  //
  // Use the standard modules for reasoning about Memory and Types
  //
  use Memory, Types;

  //
  // The main routine for computing the problem size
  //
  def computeProblemSize(numArrays: int,    // #arrays in the benchmark
			 type elemType,     // the element type of those arrays
			 returnLog2=false,  // whether to return log2(probSize)
                         memRatio=4) {      // amt of memory to use (default:1/4)
    //
    // Compute the total memory available to the benchmark using a sum
    // reduction over the amount of physical memory (in bytes) owned
    // by the set of locales on which we're running.  Then compute the
    // number of bytes we want to use as defined by memRatio and the
    // number that will be required by each index in the problem size.
    //
    const totalMem = + reduce Locales.physicalMemory(unit = MemUnits.Bytes),
          memoryTarget = totalMem / memRatio,
          bytesPerIndex = numArrays * numBytes(elemType);

    //
    // Use these values to compute a base number of indices
    //
    var numIndices = memoryTarget / bytesPerIndex;

    //
    // If the user requested a 2**n problem size, compute appropriate
    // values for numIndices and lgProblemSize
    //
    var lgProblemSize = log2(numIndices);
    if (returnLog2) {
      numIndices = 2**lgProblemSize;
      if (numIndices * bytesPerIndex <= memoryTarget) {
        numIndices *= 2;
        lgProblemSize += 1;
      }
    }

    //
    // Compute the smallest amount of memory that any locale owns
    // using a min reduction and ensure that it is sufficient to hold
    // an even portion of the problem size.
    //
    const smallestMem = min reduce Locales.physicalMemory(unit = MemUnits.Bytes);
    if ((numIndices * bytesPerIndex)/numLocales > smallestMem) then
      halt("System is too heterogeneous: blocked data won't fit into memory");

    //
    // return the problem size as requested by the callee
    //
    return if returnLog2 then lgProblemSize else numIndices;
  }

  //
  // Print out the problem size, #bytes per array, and total memory
  // required by the arrays
  //
  def printProblemSize(type elemType, numArrays, problemSize: ?psType) {
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
