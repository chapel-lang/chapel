//
// A shared module for computing the appropriate problem size for the
// HPCC benchmarks
//
module HPCCProblemSize {
  //
  // Use the standard modules for reasoning about Memory and Types
  //
  use MemDiagnostics, Types;

  //
  // The main routine for computing the problem size
  //
  proc computeProblemSize(numArrays: int,    // #arrays in the benchmark
			 type elemType,     // the element type of those arrays
                         rank=1,            // rank of the arrays
			 returnLog2=false,  // whether to return log2(probSize)
                         memFraction=4,     // fraction of mem to use (eg, 1/4)
                         type retType = int): retType { // type to return
    //
    // Compute the total memory available to the benchmark using a sum
    // reduction over the amount of physical memory (in bytes) owned
    // by the set of locales on which we're running.  Then compute the
    // number of bytes we want to use as defined by memFraction and the
    // number that will be required by each index in the problem size.
    //
    const totalMem = + reduce Locales.physicalMemory(unit = MemUnits.Bytes),
          memoryTarget = totalMem / memFraction,
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
      if rank != 1 then 
        halt("computeProblemSize() can't compute 2D 2**n problem sizes yet");
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
    if returnLog2 then
      return lgProblemSize: retType;
    else
      select rank {
        when 1 do return numIndices: retType;
        when 2 do return ceil(sqrt(numIndices)): retType;
        otherwise halt("Unexpected rank in computeProblemSize");
      }
  }

  //
  // Print out the machine configuration used to run the job
  //
  proc printLocalesTasks(gridRows = 0, gridColumns = 0) {
    write("Number of Locales = ", numLocales);
    if gridRows != 0 && gridColumns != 0 then
      write("  grid: ", gridRows, " * ", gridColumns);
    writeln();
    writeln("Tasks per locale = ", dataParTasksPerLocale);
  }

  //
  // Print out the problem size, #bytes per array, and total memory
  // required by the arrays
  //
  proc printProblemSize(type elemType, numArrays, problemSize: ?psType, 
                        param rank=1, problemSize2=problemSize) {
    const bytesPerArray = problemSize**rank * numBytes(elemType),
          totalMemInGB = (numArrays * bytesPerArray:real) / (1024**3),
          lgProbSize = log2(problemSize):psType,
          lgProbSize2 = log2(problemSize2):psType;

    write("Problem size = ", problemSize);
    for i in 2..rank do write(" x ", problemSize2);
    if (2**lgProbSize == problemSize && 2**lgProbSize2 == problemSize2) {
      write(" (2**", lgProbSize);
      for i in 2..rank do write(" x 2**", lgProbSize2);
      write(")");
    }
    writeln();
    writeln("Bytes per array = ", bytesPerArray);
    writeln("Total memory required (GB) = ", totalMemInGB);
  }
}
