// We test this - broken - piece of code
// because the compiler used to crash on it.

var DDD = {1..10};

proc main {
  var tot: int;

  forall a in DDD.myIter() with (+ reduce tot) do
    tot += a;

  writeln(tot);
}

iter DefaultRectangularDom.myIter() { yield 555; }

iter DefaultRectangularDom.myIter(param tag) {
      if chpl__testParFlag then
        chpl__testPar("default rectangular domain standalone invoked on ", ranges);
      if debugDefaultDist then
        chpl_debug_writeln("*** In domain standalone code:");

      const numTasks = if tasksPerLocale == 0 then here.maxTaskPar
                       else tasksPerLocale;
      if debugDefaultDist {
        chpl_debug_writeln("    numTasks=", numTasks, " (", ignoreRunning,
                "), minIndicesPerTask=", minIndicesPerTask);
      }
      const (numChunks, parDim) = if __primitive("task_get_serial") then
                                  (1, -1) else
                                  _computeChunkStuff(numTasks,
                                                     ignoreRunning,
                                                     minIndicesPerTask,
                                                     ranges);
      if debugDefaultDist {
        chpl_debug_writeln("    numChunks=", numChunks, " parDim=", parDim,
                " ranges(", parDim, ").size=", ranges(parDim).size);
      }

      if debugDataPar {
        chpl_debug_writeln("### numTasksPerLoc = ", numTasks, "\n" +
                "### ignoreRunning = ", ignoreRunning, "\n" +
                "### minIndicesPerTask = ", minIndicesPerTask, "\n" +
                "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n" +
                "### nranges = ", ranges);
      }

      if numChunks <= 1 {
        for i in these_help(1) {
          yield i;
        }
      } else {
        var locBlock: rank*range(intIdxType);
        for param i in 1..rank {
          locBlock(i) = offset(i)..#(ranges(i).size);
        }
        if debugDefaultDist {
          chpl_debug_writeln("*** DI: locBlock = ", locBlock);
        }
        coforall chunk in 0..#numChunks {
          var followMe: rank*range(intIdxType) = locBlock;
          const (lo,hi) = _computeBlock(locBlock(parDim).size,
                                        numChunks, chunk,
                                        locBlock(parDim)._high,
                                        locBlock(parDim)._low,
                                        locBlock(parDim)._low);
          followMe(parDim) = lo..hi;
          if debugDefaultDist {
            chpl_debug_writeln("*** DI[", chunk, "]: followMe = ", followMe);
          }
          var block: rank*range(idxType=intIdxType, stridable=stridable);
          if stridable {
            type strType = chpl__signedType(intIdxType);
            for param i in 1..rank {
              // Note that a range.stride is signed, even if the range is not
              const rStride = ranges(i).stride;
              const rSignedStride = rStride:strType;
              if rStride > 0 {
                // Since stride is positive, the following line results
                // in a positive number, so casting it to e.g. uint is OK
                const riStride = rStride:intIdxType;
                const low = ranges(i).alignedLowAsInt + followMe(i).low*riStride,
                      high = ranges(i).alignedLowAsInt + followMe(i).high*riStride,
                      stride = rSignedStride;
                block(i) = low..high by stride;
              } else {
                // Stride is negative, so the following number is positive.
                const riStride = (-rStride):intIdxType;
                const low = ranges(i).alignedHighAsInt - followMe(i).high*riStride,
                      high = ranges(i).alignedHighAsInt - followMe(i).low*riStride,
                      stride = rSignedStride;
                block(i) = low..high by stride;
              }
            }
          } else {
            for  param i in 1..rank do
              block(i) = ranges(i)._low+followMe(i).low:intIdxType..ranges(i)._low+followMe(i).high:intIdxType;
          }
          for i in these_help(1, block) {
            yield chpl_intToIdx(i);
          }
        }
      }
    }
