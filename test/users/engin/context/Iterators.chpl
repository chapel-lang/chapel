module Iterators {

  config const taskPerLoc = 2;
  config const doDebug = false;

  proc debug(args...) {
    if doDebug then writeln((...args));
  }

  module SimpleOneDim {
    use Iterators;
    use ChapelContextSupport;


    iter simpleOneDim(n) {
      const iterRange = 0..<n;
      foreach i in iterRange do yield i;
    }

    iter simpleOneDim(param tag: iterKind, n) where tag==iterKind.leader {
      coforall l in Locales {
        var outerCtx = new Context(rank=1, taskId=l.id, numTasks=numLocales);

        on l {
          var onCtx = new Context(rank=1, taskId=0, numTasks=1);

          const perLocSize = n/numLocales;
          const localeLow = l.id * perLocSize;
          const localeHigh = localeLow+perLocSize-1;

          debug(here, " ", localeLow, " " , localeHigh);

          coforall taskId in 0..<taskPerLoc {
            var innerCtx = new Context(rank=1, taskId=taskId, numTasks=taskPerLoc);

            const perTaskSize = perLocSize/taskPerLoc;
            const taskLow = localeLow+(taskId*perTaskSize);
            const taskHigh = taskLow+perTaskSize-1;

            debug(here, ":", taskId, " ", taskLow, " " , taskHigh);

            yield taskLow..taskHigh;
          }
        }
      }
    }

    iter simpleOneDim(param tag: iterKind, n, followThis) where tag==iterKind.follower {
      foreach i in followThis do yield i;
    }
  }

  module TertiaryDRDomIterators {
    use BlockDist;
    use DSIUtil;
    use ChapelContextSupport;

    iter BlockDom.customThese(param tag: iterKind) where tag == iterKind.leader {
      const maxTasks = dist.dataParTasksPerLocale;
      const ignoreRunning = dist.dataParIgnoreRunningTasks;
      const minSize = dist.dataParMinGranularity;
      const wholeLow = whole.lowBound;

      // If this is the only task running on this locale, we don't want to
      // count it when we try to determine how many tasks to use.  Here we
      // check if we are the only one running, and if so, use
      // ignoreRunning=true for this locale only.  Obviously there's a bit
      // of a race condition if some other task starts after we check, but
      // in that case there is no correct answer anyways.
      //
      // Note that this code assumes that any locale will only be in the
      // targetLocales array once.  If this is not the case, then the
      // tasks on this locale will *all* ignoreRunning, which may have
      // performance implications.
      const hereId = here.id;
      const hereIgnoreRunning = if here.runningTasks() == 1 then true
        else ignoreRunning;
      coforall (locDom, locDomIdx) in zip(locDoms, locDoms.domain) do on locDom {
        var onCtx = new Context(rank=rank, taskId=locDomIdx, numTasks=locDoms.domain.shape);

        const myIgnoreRunning = if here.id == hereId then hereIgnoreRunning
                                                     else ignoreRunning;
        // Use the internal function for untranslate to avoid having to do
        // extra work to negate the offset
        type strType = chpl__signedType(idxType);
        const tmpBlock = locDom.myBlock.chpl__unTranslate(wholeLow);
        var locOffset: rank*idxType;
        for param i in 0..tmpBlock.rank-1 {
          const stride = tmpBlock.dim(i).stride;
          if stride < 0 && strType != idxType then
            halt("negative stride not supported with unsigned idxType");
          // (since locOffset is unsigned in that case)
          locOffset(i) = tmpBlock.dim(i).first / stride:idxType;
        }
        // Forward to defaultRectangular
        for followThis in tmpBlock.customThese(iterKind.leader, maxTasks,
            myIgnoreRunning, minSize, locOffset) do
          yield followThis;
      }
    }

    //
    // TODO: Abstract the addition of low into a function?
    // Note relationship between this operation and the
    // order/position functions -- any chance for creating similar
    // support? (esp. given how frequent this seems likely to be?)
    //
    // TODO: Is there some clever way to invoke the leader/follower
    // iterator on the local blocks in here such that the per-core
    // parallelism is expressed at that level?  Seems like a nice
    // natural composition and might help with my fears about how
    // stencil communication will be done on a per-locale basis.
    //
    // TODO: Can we just re-use the DefaultRectangularDom follower here?
    //
    iter BlockDom.customThese(param tag: iterKind, followThis) where tag == iterKind.follower {
      if chpl__testParFlag then
        chpl__testParWriteln("Block domain follower invoked on ", followThis);

      var t: rank*range(idxType, strides = chpl_strideProduct(whole.strides,
            chpl_strideUnion(followThis)));
      type strType = chpl__signedType(idxType);
      for param i in 0..rank-1 {
        var stride = whole.dim(i).stride: strType;
        // not checking here whether the new low and high fit into idxType
        var low = (stride * followThis(i).lowBound:strType):idxType;
        var high = (stride * followThis(i).highBound:strType):idxType;
        t(i) = ((low..high by stride:strType) + whole.dim(i).low by followThis(i).stride:strType).safeCast(t(i).type);
      }
      for i in {(...t)} {
        yield i;
      }
    }

    iter DefaultRectangularDom.customThese(param tag: iterKind,
               tasksPerLocale = dataParTasksPerLocale,
               ignoreRunning = dataParIgnoreRunningTasks,
               minIndicesPerTask = dataParMinGranularity,
               offset=createTuple(rank, chpl_integralIdxType, 0:chpl_integralIdxType))
      where tag == iterKind.leader {


      const numSublocs = here._getChildCount();

      if localeModelPartitionsIterationOnSublocales && numSublocs != 0 {
        var dptpl = if tasksPerLocale==0 then here.maxTaskPar
                    else tasksPerLocale;
        if !ignoreRunning {
          const otherTasks = here.runningTasks() - 1; // don't include self
          dptpl = if otherTasks < dptpl then (dptpl-otherTasks):int else 1;
        }
        // Make sure we don't use more sublocales than the numbers of
        // tasksPerLocale requested
        const numSublocTasks = min(numSublocs, dptpl);
        // For serial tasks, we will only have a single chunk
        const (numChunks, parDim) = if __primitive("task_get_serial") then
                                    (1, 0) else
                                    _computeChunkStuff(numSublocTasks,
                                                       ignoreRunning=true,
                                                       minIndicesPerTask,
                                                       ranges);
        if debugDataParNuma {
          chpl_debug_writeln("### numSublocs = ", numSublocs, "\n",
                 "### numTasksPerSubloc = ", numSublocTasks, "\n",
                  "### ignoreRunning = ", ignoreRunning, "\n",
                  "### minIndicesPerTask = ", minIndicesPerTask, "\n",
                  "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n",
                  "### nranges = ", ranges);
        }

        coforall chunk in 0..#numChunks { // make sure coforall on can trigger
          local do on here._getChild(chunk) {
            if debugDataParNuma {
              if chunk!=chpl_getSubloc() then
                chpl_debug_writeln("*** ERROR: ON WRONG SUBLOC (should be ", chunk,
                                   ", on ", chpl_getSubloc(), ") ***");
            }
            // Divide the locale's tasks approximately evenly
            // among the sublocales
            const numSublocTasks = (if chunk < dptpl % numChunks
                                    then dptpl / numChunks + 1
                                    else dptpl / numChunks);
            var locBlock: rank*range(chpl_integralIdxType);
            for param i in 0..rank-1 do
              locBlock(i) = offset(i)..#(ranges(i).sizeAs(chpl_integralIdxType));
            var followMe: rank*range(chpl_integralIdxType) = locBlock;
            const (lo,hi) = _computeBlock(locBlock(parDim).sizeAs(chpl_integralIdxType),
                                          numChunks, chunk,
                                          locBlock(parDim)._high,
                                          locBlock(parDim)._low,
                                          locBlock(parDim)._low);
            followMe(parDim) = lo..hi;
            const (numChunks2, parDim2) = _computeChunkStuff(numSublocTasks,
                                                             ignoreRunning=true,
                                                             minIndicesPerTask,
                                                             followMe);
            coforall chunk2 in 0..#numChunks2 {
              var locBlock2: rank*range(chpl_integralIdxType);
              for param i in 0..rank-1 do
                locBlock2(i) = followMe(i).lowBound..followMe(i).highBound;
              var followMe2: rank*range(chpl_integralIdxType) = locBlock2;
              const low  = locBlock2(parDim2)._low,
                high = locBlock2(parDim2)._high;
              const (lo,hi) = _computeBlock(locBlock2(parDim2).sizeAs(chpl_integralIdxType),
                                            numChunks2, chunk2,
                                            high, low, low);
              followMe2(parDim2) = lo..hi;
              if debugDataParNuma {
                chpl_debug_writeln("### chunk = ", chunk, "  chunk2 = ", chunk2, "  ",
                        "followMe = ", followMe, "  followMe2 = ", followMe2);
              }
              yield followMe2;
            }
          }
        }
      } else {

        if debugDefaultDist then
          chpl_debug_writeln("*** In domain/array leader code:"); // this = ", this);
        const numTasks = if tasksPerLocale==0 then here.maxTaskPar
                         else tasksPerLocale;

        if debugDefaultDist then
          chpl_debug_writeln("    numTasks=", numTasks, " (", ignoreRunning,
                  "), minIndicesPerTask=", minIndicesPerTask);

        const (numChunks, parDim) = if __primitive("task_get_serial") then
                                    (1, 0) else
                                    _computeChunkStuff(numTasks,
                                                       ignoreRunning,
                                                       minIndicesPerTask,
                                                       ranges);
        if debugDefaultDist then
          chpl_debug_writeln("    numChunks=", numChunks, " parDim=", parDim,
                  " ranges(", parDim, ").size=", ranges(parDim).sizeAs(uint));

        if debugDataPar {
          chpl_debug_writeln("### numTasksPerLoc = ", numTasks, "\n",
                 "### ignoreRunning = ", ignoreRunning, "\n",
                 "### minIndicesPerTask = ", minIndicesPerTask, "\n",
                 "### numChunks = ", numChunks, " (parDim = ", parDim, ")\n",
                  "### nranges = ", ranges);
        }

        var locBlock: rank*range(chpl_integralIdxType);
        for param i in 0..rank-1 do
          locBlock(i) = offset(i)..#(ranges(i).sizeAs(chpl_integralIdxType));
        if debugDefaultDist then
          chpl_debug_writeln("*** DI: locBlock = ", locBlock);
        coforall chunk in 0..#numChunks {
          var innerCtx = new Context(rank=1, taskId=chunk, numTasks=numChunks);

          var followMe: rank*range(chpl_integralIdxType) = locBlock;
          const (lo,hi) = _computeBlock(locBlock(parDim).sizeAs(chpl_integralIdxType),
                                        numChunks, chunk,
                                        locBlock(parDim)._high,
                                        locBlock(parDim)._low,
                                        locBlock(parDim)._low);
          followMe(parDim) = lo..hi;
          if debugDefaultDist then
            chpl_debug_writeln("*** DI[", chunk, "]: followMe = ", followMe);
          yield followMe;
        }
      }
    }

    iter BlockDom.customThese() {
      for i in whole do
        yield i;
    }

    /* Yield the domain indices */
    iter _domain.customThese() {
      for i in _value.customThese() {
        yield i;
      }
    }

    @chpldoc.nodoc
    iter _domain.customThese(param tag: iterKind)
      where tag == iterKind.standalone &&
            __primitive("resolves", _value.these(tag=tag)) {
      for i in _value.customThese(tag) do
        yield i;
    }
    @chpldoc.nodoc
    iter _domain.customThese(param tag: iterKind)
      where tag == iterKind.leader {
      // If I use forall here, it says
      //   error: user invocation of a parallel iterator should not supply tag
      //   arguments -- they are added implicitly by the compiler
      for followThis in _value.customThese(tag) do
        yield followThis;
    }
    @chpldoc.nodoc
    iter _domain.customThese(param tag: iterKind, followThis, param fast: bool = false)
      where tag == iterKind.follower {

      if __primitive("resolves", _value.customThese(tag=tag, followThis, fast=fast)) {
        for i in _value.customThese(tag=tag, followThis, fast=fast) do
          yield i;
      } else {
        for i in _value.customThese(tag, followThis) do
          yield i;
      }
    }

  }
}
