// Useful functions for implementing distributions

pragma "inline"
proc getDataParTasksPerLocale() {
  return dataParTasksPerLocale;
}

pragma "inline"
proc getDataParIgnoreRunningTasks() {
  return dataParIgnoreRunningTasks;
}

pragma "inline"
proc getDataParMinGranularity() {
  return dataParMinGranularity;
}

//
// helper functions for determining the number of chunks and the
//   dimension to chunk over
//
proc _computeChunkStuff(maxTasks, ignoreRunning, minSize, ranges) {
  param rank=ranges.size;
  var numElems: uint(64) = 1;
  for param i in 1..rank do {
    numElems *= ranges(i).length:uint(64);
  }

  var numChunks = _computeNumChunks(maxTasks, ignoreRunning, minSize, numElems);

  // Dimension to parallelize (eventually should "block" thespace)
  var parDim = -1;
  var maxDim = -1;
  var maxElems = min(uint(64));
  // break/continue don't work with param loops (known future)
  for /* param */ i in 1..rank do {
    if ranges(i).length:uint(64) >= numChunks {
      parDim = i;
      break;
    }
    if ranges(i).length:uint(64) > maxElems {
      maxElems = ranges(i).length:uint(64);
      maxDim = i;
    }
  }
  if parDim == -1 then
    parDim = maxDim;

  return (numChunks, parDim);
}

proc _computeNumChunks(maxTasks, ignoreRunning, minSize, numElems) {
  assert(numElems >= 0);
  const unumElems = numElems:uint(64);
  const runningTasks = here.runningTasks();
  var numChunks: uint(64) =
    if ignoreRunning then maxTasks:uint(64)
    else if runningTasks-1 < maxTasks // don't include self
      then (maxTasks-runningTasks+1):uint(64)
      else 1:uint(64);

  if minSize > 0 then
    // This is approximate
    while (unumElems < minSize:uint(64)*numChunks) && (numChunks > 1) {
        numChunks -= 1;
    }

  if numChunks > unumElems then numChunks = unumElems;

  return numChunks;
}

// How many tasks should be spawned to service numElems elements.
proc _computeNumChunks(numElems) {
  // copy some machinery from DefaultRectangularDom
  var numTasks = if dataParTasksPerLocale==0
                 then here.numCores
                 else dataParTasksPerLocale;
  var ignoreRunning = dataParIgnoreRunningTasks;
  var minIndicesPerTask = dataParMinGranularity;
  var numChunks = _computeNumChunks(numTasks, ignoreRunning,
                                    minIndicesPerTask, numElems)
                  :numElems.type;
  return numChunks;
}

// Divide 1..numElems into (almost) equal numChunk pieces
// and return myChunk-th piece.
proc _computeChunkStartEnd(numElems, numChunks, myChunk) {
  var div = numElems / numChunks;
  var rem = numElems % numChunks;

  if myChunk <= rem then {
    // (div+1) elements per chunk
    var endIx = myChunk * (div + 1);
    //writeln("yielding ", endIx - div, "..", endIx);
    return (endIx - div, endIx);
  } else {
    // (div) elements per chunk
    var startIx1 = numElems - (numChunks - myChunk + 1) * div;
    //writeln("yielding ", startIx1 + 1, "..", startIx1 + div);
    return (startIx1 + 1, startIx1 + div);
  }
}

//
// helper function for blocking index ranges
//
proc _computeBlock(numelems, numblocks, blocknum, wayhi,
                  waylo=0:wayhi.type, lo=0:wayhi.type) {
  proc intCeilXDivByY(x, y) return ((x + (y-1)) / y);

  if numelems == 0 then
    return (1:lo.type, 0:lo.type);

  const blo =
    if blocknum == 0 then waylo
    else lo + intCeilXDivByY(numelems:lo.type * blocknum:lo.type, numblocks:lo.type);
  const bhi =
    if blocknum == numblocks - 1 then wayhi
    else lo + intCeilXDivByY(numelems:lo.type * (blocknum+1):lo.type, numblocks:lo.type) - 1;

  return (blo, bhi);
}

//
// naive routine for dividing numLocales into rank factors
//
proc _factor(param rank: int, value) {
  var factors: rank*int;
  for param i in 1..rank do
    factors(i) = 1;
  if value >= 1 {
    var iv = value;
    var factor = 1;
    while iv > 1 {
      for i in 2..iv {
        if iv % i == 0 {
          var j = 1;
          for i in 2..rank {
            if factors(i) < factors(j) then
              j = i;
          }
          factors(j) *= i;
          iv = iv / i;
          break;
        }
      }
    }
  }
  for i in 1..rank do
    for j in i+1..rank do
      if factors(i) < factors(j) then
        factors(i) <=> factors(j);
  return factors;
}

//
// Returns a new default rectangular domain of the same rank, index
// type, and shape of 'dom' but for which the indices in each
// dimension start at zero and have unit stride.
//
proc computeZeroBasedDomain(dom: domain) {
  proc helper(first, rest...) {
    if rest.size > 1 then
      return (0..#first.length, (...helper((...rest))));
    else
      return (0..#first.length, 0..#rest(1).length);
  }
  if dom.rank > 1 then
    return [(...helper((...dom.dims())))];
  else
    return [0..#dom.dim(1).length];
}

//
// setupTargetLocalesArray
//
proc setupTargetLocalesArray(targetLocDom, targetLocArr, specifiedLocArr) {
  param rank = targetLocDom.rank;
  if rank != 1 && specifiedLocArr.rank == 1 {
    const factors = _factor(rank, specifiedLocArr.numElements);
    var ranges: rank*range;
    for param i in 1..rank do
      ranges(i) = 0..#factors(i);
    targetLocDom = [(...ranges)];
    targetLocArr = reshape(specifiedLocArr, targetLocDom);
  } else {
    if specifiedLocArr.rank != rank then
      compilerError("specified target array of locales must equal 1 or distribution rank");
    var ranges: rank*range;
    for param i in 1..rank do
      ranges(i) = 0..#specifiedLocArr.domain.dim(i).length;
    targetLocDom = [(...ranges)];
    targetLocArr = specifiedLocArr;
  }
}
