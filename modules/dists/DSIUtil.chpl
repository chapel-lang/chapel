// Useful functions for implementing distributions

pragma "inline"
def getDataParTasksPerLocale() {
  return dataParTasksPerLocale;
}

pragma "inline"
def getDataParIgnoreRunningTasks() {
  return dataParIgnoreRunningTasks;
}

pragma "inline"
def getDataParMinGranularity() {
  return dataParMinGranularity;
}

//
// helper functions for determining the number of chunks and the
//   dimension to chunk over
//
def _computeChunkStuff(maxTasks, ignoreRunning, minSize, ranges) {
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

def _computeNumChunks(maxTasks, ignoreRunning, minSize, numElems) {
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


//
// helper function for blocking index ranges
//
def _computeBlock(numelems, numblocks, blocknum, wayhi,
                  waylo=0:wayhi.type, lo=0:wayhi.type) {
  def intCeilXDivByY(x, y) return ((x + (y-1)) / y);

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
def _factor(param rank: int, value) {
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
/*  if rank == 2 {
    factors(1)=value;
    factors(2)=1;
  }
*/
  return factors;
}

//
// Returns a new default arithmetic domain of the same rank, index
// type, and shape of 'dom' but for which the indices in each
// dimension start at zero and have unit stride.
//
def computeZeroBasedDomain(dom: domain) {
  def helper(first, rest...) {
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
def setupTargetLocalesArray(targetLocDom, targetLocArr, specifiedLocArr) {
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

// Function to read a 2D block from a file, so it can be used to read a
// Distribution from file with:
// 		var bb=read2DBlock(infile);
//		var Distrib = new dmap(new Block(rank=2, bb));
//  And latter to read and define a Domain over that distribution from file
//  with :
//		bb=read2DBlock(infile);
//		var Dom2: domain(2) dmapped Distrib = bb;
//
// They can be written to file, so they can be read latter, using:
//		outfile.write(Dist);
//		outfile.write(Dom);


def read2DBlock(f : file) {
  var gres=0:int(64);
  var gerr=0:int;

//  var fpos=f.chpl_ftell();
//  writeln("readBinBlock begin, ftell:",fpos);

  var rank:int;
  binfread(rank,4,1,f._fp,gres,gerr);
//  writeln("Read Block rank:",rank);

  var rr:int;
  var bb:domain(2);
  var low: [1..2] int;
  var high: [1..2] int;
  var stride: [1..2] int;

  for dim in 1..rank do {
    binfread(low[dim],4,1,f._fp,gres,gerr);
    binfread(high[dim],4,1,f._fp,gres,gerr);
    binfread(stride[dim],4,1,f._fp,gres,gerr);
//    writeln("Read Block dim:",dim," low   : ",low[dim]);
//    writeln("Read Block dim:",dim," high  : ",high[dim]);
//    writeln("Read Block dim:",dim," stride: ",stride[dim]);
  }

// If there is an error reading it will fail when defining the domain.
  bb=[low[1]..high[1] by stride[1],low[2]..high[2] by stride[2]];
// writeln("readBlock bb:",bb);
//  fpos=f.chpl_ftell();
//  writeln("readBinBlock end, ftell:",fpos);
  return bb;
}

