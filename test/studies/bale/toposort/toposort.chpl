use Random;
use Time;
use LayoutCS;
config param enableRuntimeDebugging = true;
config const debugAll : bool = false;
config const debugTopo = debugAll;
config const debugTopoTasking = debugAll || debugTopo;
config const debugCreateSparseUTDomain = debugAll;
config const debugPermute = debugAll;
config param useDimIter = true;
config param useDimIterRow = false; // not currently supported on master
config param useDimIterCol = useDimIter;

record Lock {
  var lock$ : sync bool;

  proc init(){
    this.complete();
    this.lock$.writeEF(true);
  }

  proc lock(){
    lock$.readFE();
  }

  proc unlock(){
    lock$.writeEF(true);
  }

  proc forceUnlock(){
    lock$.writeXF(true);
  }
}

class PermutationMap {
  type idxType;
  param rank = 2;
  var rowDom : domain(1);
  var columnDom : domain(1);
  var rowMap : [rowDom] idxType;
  var columnMap : [columnDom] idxType;

  proc init( rowMap : [] ?idxType, columnMap : [] idxType ){
    this.idxType = idxType;
    this.rowDom = rowMap.domain;
    this.columnDom = columnMap.domain;
    this.rowMap = rowMap;
    this.columnMap = columnMap;
  }

  inline proc map( idx : rank*idxType ) : rank*idxType {
    return (rowMap[idx[1]], columnMap[idx[2]]);
  }

  inline proc inverseMap( idx : rank*idxType ) : rank*idxType {
    return ( linearSearch(rowMap, idx[1]), linearSearch(rowMap, idx[1]) );
  }

  inline proc this( idx : rank*idxType ) : rank*idxType {
    return map( idx );
  }

  proc createInverseMap() : owned PermutationMap(idxType) {
    var inverseRowMap : [rowMap.domain] rowMap.eltType;
    var inverseColumnMap : [columnMap.domain] columnMap.eltType;

    for i in rowMap.domain {
      inverseRowMap[ rowMap[i] ] = i;
    }

    for i in columnMap.domain {
      inverseColumnMap[ columnMap[i] ] = i;
    }

    return new owned PermutationMap( inverseRowMap, inverseColumnMap );
  }

  iter these( onDomain : domain ) : rank*idxType
  where onDomain.rank == 2
  {
    for idx in onDomain do yield this.map( idx );
  }

  iter inverseThese( onDomain : domain ) : rank*idxType
  where onDomain.rank == 2
  {
    for idx in onDomain do yield inverseMap( idx );
  }


  // TODO make parallel
  iter these(param tag : iterKind, onDomain : domain) : rank*idxType
  where tag == iterKind.standalone && onDomain.rank == 2
  {
    for idx in this.these( onDomain ) do yield idx;
  }

  // TODO leader follower iterator

  proc writeThis( f ){
    const maxVal = max( (max reduce rowMap), (max reduce columnMap) ) : string;
    const minVal = min( (min reduce rowMap), (min reduce columnMap) ) : string;
    const padding = max( maxVal.length, minVal.length );
    const formatString = "%%%nn -> %%%nn".format( max(2,padding), padding );
    const inSpace = max(padding-2,0);
    f <~> "Row map\n";
    for i in 0..#inSpace do f <~> " ";
    f <~> "in -> out\n";
    for i in rowDom {
      f <~> formatString.format( i, rowMap[i] ) <~> "\n";
    }
    f <~> "Column map\n" ;
    for i in 0..#inSpace do f <~> " ";
    f <~> "in -> out\n";
    for i in columnDom {
      f <~> formatString.format( i, columnMap[i] ) <~> "\n";
    }
  }

  proc permuteDomain( D : domain )
  where D.rank == 2 && isSparseDom( D )
  {
    // Timer for debugging purposes
    var timer : Timer;

    // resulting domain after permutation
    var sD : D.type;

    timer.start();
    var sDBulk : [1..#D.size] D.rank*D.idxType;
    // TODO make parallel
    for (pos,idx) in zip(1..#D.size, D) {
      sDBulk[pos]= this[idx];
    }
    sD.bulkAdd( sDBulk );
    timer.stop();
    if enableRuntimeDebugging && debugPermute then writeln( "Add domain ", timer.elapsed() );
    timer.clear();

    return sD;
  }
}

proc createRandomPermutationMap( D : domain, seed : int ) : owned PermutationMap(D.idxType)
where D.rank == 2
{
  var rowMap : [D.dim(1)] D.idxType = D.dim(1);
  var columnMap : [D.dim(2)] D.idxType = D.dim(2);
  //use seed to create two new seeds, one for each shuffle
  var randStreamSeeded = new owned RandomStream(int, seed);
  const seed1 = randStreamSeeded.getNext() | 1;
  const seed2 = randStreamSeeded.getNext() | 1;
  shuffle( rowMap, seed = seed1 );
  shuffle( columnMap , seed = seed2);
  return new owned PermutationMap( rowMap, columnMap );
}

record TopoSortResult {
  type idxType;
  var permutationMap : shared PermutationMap(idxType);
  var timerDom : domain(string);
  var timers : [timerDom] Timer;

  proc init(type idxType){
    this.idxType = idxType;
    this.permutationMap = nil;
    this.timerDom = {"whole","initialization","toposort"};
  }
}

proc toposortParallel( D : domain, numTasks : int = here.maxTaskPar )
where D.rank == 2
{
  if numTasks < 1 then halt("Must run with numTaks >= 1");

  var result = new TopoSortResult(D.idxType);
  result.timers["whole"].start();

  var numDiagonals = min( D.dim(1).size, D.dim(2).size );

  var rowMap : [D.dim(1)] D.idxType = [i in D.dim(1)] -1;
  var columnMap : [D.dim(2)] D.idxType = [i in D.dim(2)] -1;

  var rowSum : [D.dim(1)] atomic int;
  var rowCount : [D.dim(1)] atomic int;
  var workQueue : list(D.idxType);
  var sharedLock = new Lock();

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  forall row in D.dim(1) with (ref rowSum, ref rowCount, ref workQueue, ref sharedLock) {
    // Accumulate task locally, then write at end.
    var count = 0;
    var sum = 0;

    if enableRuntimeDebugging && debugTopo then writeln( "initializing row ", row );
    if useDimIterCol {
     // compilerWarning("iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        count += 1;
        sum += col;
      }
    } else {
      // compilerWarning("iterating over columns in init with dim");
      for col in D.dim(2) {
        if D.member((row,col)) {
          count += 1;
          sum += col;
        }
      }
    }
    if count == 1 {
      sharedLock.lock();
      workQueue.push_back( row );
      sharedLock.unlock();
    }

    rowCount[row].write( count);
    rowSum[row].write( sum );
  }
  result.timers["initialization"].stop();

  if enableRuntimeDebugging && debugTopo {
    writeln( "initial workQueue ", workQueue );
    writeln( "initial rowSum    ", rowSum );
    writeln( "initial rowCount  ", rowCount );
  }

  // insert position along diagonal from (N,N)
  // must be accessed under sharedLock
  var diagonalPosition : int = 0;
  // Values for work signal
  enum TaskSignal {
    kill = min(int), // no more work signal, die when recieving
    fail = kill : int + 1, // signal that someone failed, and so should exit with error
    wake = 1 // more work signal, pop from stack and continue with recieving
  };
  // This will signal tasks to get work, and keeps track of queue size;
  var workSignal : sync TaskSignal;
  // initialize work signal.
  // sanity check: if queue is empty, set to kill signal.
  // (I dont think this-^ is possible, but it could in the future?)
  workSignal.writeEF( if workQueue.size > 0 then TaskSignal.wake else TaskSignal.kill );
  if enableRuntimeDebugging && debugTopoTasking then writeln( "Spawning tasks ", 1..#numTasks );

  result.timers["toposort"].start();
  coforall taskId in 1..#numTasks
    with (ref diagonalPosition, ref rowSum, ref rowCount, ref workQueue, ref sharedLock)
  {
    if enableRuntimeDebugging && debugTopoTasking then writeln(taskId, " spawning." );
    // continue looping until kill or fail signal is recieved
    var continueWorking = true;
    while continueWorking {
      if enableRuntimeDebugging && debugTopoTasking then writeln( taskId, " waiting." );
      // wait for work and get signal
      var signal = workSignal;
      if enableRuntimeDebugging && debugTopoTasking then writeln( taskId, " recieved signal: ", signal );
      // if signal is kill signal relay message and exit
      select signal {
        when TaskSignal.kill {
          workSignal.writeEF( TaskSignal.kill );
          continueWorking = false;
          if enableRuntimeDebugging && debugTopoTasking then writeln( taskId, " killed." );
          break; // break out of loop;
        }
        when TaskSignal.fail {
          workSignal.writeXF( TaskSignal.fail );
          continueWorking = false;
          if enableRuntimeDebugging && debugTopoTasking then writeln( taskId, " failing out.");
          break; // break out of loop;
        }
        otherwise { /* no need to popogate signal */ }
      }

      // Critical section
      sharedLock.lock();
      if enableRuntimeDebugging && debugTopo {
        writeln(
          "========================",
          "\ntaskId ",  taskId,
          "\nworkQueue ", workQueue,
          "\nrowSum    ", rowSum,
          "\nrowCount  ", rowCount,
          "\nrowMap    ", rowMap,
          "\ncolumnMap ", columnMap,
          "\n========================"
        );
      }

      var queueSize = workQueue.size;
      // if queueSize is < 1, someone stole our work. unlock and rewait
      if queueSize < 1 {
        if enableRuntimeDebugging && debugTopoTasking then writeln( taskId, " queue size is ", queueSize);
        sharedLock.unlock();
        continue;
      }

      // safely get row and diagonal position
      var localDiagonalPosition = diagonalPosition;
      diagonalPosition += 1; // increment to next position
      var swapRow = workQueue.pop_front();
      queueSize = workQueue.size;

      // if there are no more diagonal to place, then there is no work, send kill signal
      if enableRuntimeDebugging && debugTopo then writeln( (taskId, localDiagonalPosition, diagonalPosition) );
      if diagonalPosition >= numDiagonals {
        if enableRuntimeDebugging && debugTopoTasking then writeln( taskId, " sending kill signal." );
        workSignal.writeXF( TaskSignal.kill );
        continueWorking = false;
      } else if queueSize > 0 {
        if enableRuntimeDebugging && debugTopoTasking then writeln( taskId, " sending work signal." );
        workSignal.writeXF( TaskSignal.wake );
      }
      sharedLock.unlock();

      // get non-zero column
      var swapColumn = rowSum[swapRow].read();

      // permute this row to the diagonal
      rowMap[swapRow] = D.dim(1).high - localDiagonalPosition;
      columnMap[swapColumn] = D.dim(2).high - localDiagonalPosition;

      if enableRuntimeDebugging && debugTopo then writeln( "Swaping ", (swapRow,swapColumn), " -> ", (rowMap[swapRow], columnMap[swapColumn]) );

      // foreach row along the swapped column who has a nonzero at (row, swapColumn)
      // remove swapColumn from rowSum and reduce rowCount
      if useDimIterRow {
        // compilerWarning("iterating over rows in kernel with dimIter");
        for row in D.dimIter(1,swapColumn) {
          var previousRowCount = rowCount[row].fetchAdd( -1 );
          rowSum[row].add( -swapColumn );
          // if previousRowCount = 2 (ie rowCount[row] == 1)
          if previousRowCount == 2 {
            sharedLock.lock();
            if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
            workQueue.push_back( row );
            sharedLock.unlock();
            workSignal.writeXF( TaskSignal.wake );
          }
        }
      } else {
        // compilerWarning("iterating over rows in kernel with dim");
        for row in D.dim(1) {
          if D.member((row, swapColumn)) {
            var previousRowCount = rowCount[row].fetchAdd( -1 );
            rowSum[row].add( -swapColumn );
            // if rowCount[row] == 1
            if previousRowCount == 2 {
              sharedLock.lock();
              if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
              workQueue.push_back( row );
              sharedLock.unlock();
              workSignal.writeXF( TaskSignal.wake );
            }
          }
        }
      }

    } // while continueWorking
    if enableRuntimeDebugging && debugTopoTasking then writeln( taskId, " completed." );
  } // coforall tasks
  result.timers["toposort"].stop();
  var signal = workSignal;
  if signal == TaskSignal.fail {
    writeln("Recieved fail signal at some point in execution.");
    exit( -1 );
  }

  result.permutationMap = new owned PermutationMap( rowMap, columnMap );

  result.timers["whole"].stop();

  return result;
}

proc toposortSerial( D : domain )
where D.rank == 2
{
  var result = new TopoSortResult(D.idxType);
  result.timers["whole"].start();

  var numDiagonals = min( D.dim(1).size, D.dim(2).size );

  var rowMap : [D.dim(1)] D.idxType = [i in D.dim(1)] -1;
  var columnMap : [D.dim(2)] D.idxType = [i in D.dim(2)] -1;

  var rowSum : [D.dim(1)] int;
  var rowCount : [D.dim(1)] int;
  var workQueue : list(D.idxType);

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  for row in D.dim(1) {
    if enableRuntimeDebugging && debugTopo then writeln( "initializing row ", row );
    if useDimIterCol {
     // compilerWarning("iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        rowCount[row] += 1;
        rowSum[row] += col;
      }
    } else {
      // compilerWarning("iterating over columns in init with dim");
      for col in D.dim(2) {
        if D.member((row,col)) {
          rowCount[row] += 1;
          rowSum[row] += col;
        }
      }
    }

    if rowCount[row] == 1 {
      workQueue.push_back( row );
    }
  }
  result.timers["initialization"].stop();

  if enableRuntimeDebugging && debugTopo {
    writeln( "initial workQueue ", workQueue );
    writeln( "initial rowSum    ", rowSum );
    writeln( "initial rowCount  ", rowCount );
  }

  // insert position along diagonal from (N,N)
  var diagonalPosition : int = 0;

  result.timers["toposort"].start();
  while workQueue.size > 0 {
    if enableRuntimeDebugging && debugTopo {
      writeln(
        "========================",
        "\nworkQueue ", workQueue,
        "\nrowSum    ", rowSum,
        "\nrowCount  ", rowCount,
        "\nrowMap    ", rowMap,
        "\ncolumnMap ", columnMap,
        "\n========================"
      );
    }

    // get work row
    var swapRow = workQueue.pop_front();

    // get non-zero column
    var swapColumn = rowSum[swapRow];

    // permute this row to the diagonal
    rowMap[swapRow] = D.dim(1).high - diagonalPosition;
    columnMap[swapColumn] = D.dim(2).high - diagonalPosition;
    diagonalPosition += 1; // increment to next position

    if enableRuntimeDebugging && debugTopo then writeln( "Swaping ", (swapRow,swapColumn), " -> ", (rowMap[swapRow], columnMap[swapColumn]) );

    // foreach row along the swapped column who has a nonzero at (row, swapColumn)
    // remove swapColumn from rowSum and reduce rowCount
    if useDimIterRow {
      // compilerWarning("iterating over rows in kernel with dimIter");
      for row in D.dimIter(1,swapColumn) {
        rowCount[row] -= 1;
        rowSum[row] -= swapColumn;
        if rowCount[row] == 1 {
          if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
          workQueue.push_back( row );
        }
      }
    } else {
      // compilerWarning("iterating over rows in kernel with dim");
      for row in D.dim(1) {
        if D.member((row, swapColumn)) {
          rowCount[row] -= 1;
          rowSum[row] -= swapColumn;
          if rowCount[row] == 1 {
            if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
            workQueue.push_back( row );
          }
        }
      }
    }

  } // while work in queue
  result.timers["toposort"].stop();

  result.permutationMap = new owned PermutationMap( rowMap, columnMap );
  result.timers["whole"].stop();
  return result;
}

proc createSparseUpperTriangluarDomain( D : domain(2), density : real, seed : int ) {
  // Must be square matrix, uniformly dimensioned dense domain
  if D.dim(1) != D.dim(2) then halt("Domain provided to createSparseUpperTriangluarDomain is not square.");
  const N = D.dim(1).size;
  const minDensity : real = 1.0/N;
  const maxDensity : real = (N+1.0)/(2.0*N);

  // assert we have been given a legal density;
  if density < minDensity then halt( "Specified density (%n) is less than minimum density (%n) for N (%n)".format( density, minDensity, N));
  if density > maxDensity then halt( "Specified density (%n) is greater than maximum density (%n) for N (%n)".format( density, maxDensity, N));

  // number of elements in complete UT matrix
  var numberNonZerosInFullUTDomain : int = (( N * N + N)/2.0) : int;
  // number of elements added
  var numberNonZerosAddedInUT : int = max( N, floor( N*N*density ) ) : int;
  // number of non-diagonal elements added
  var numberNonZerosAddedInStrictlyUT : int = numberNonZerosAddedInUT - N;

  if enableRuntimeDebugging && debugCreateSparseUTDomain {
    writeln( "Dense: ", numberNonZerosInFullUTDomain );
    writeln( "Added: ", numberNonZerosAddedInUT );
    writeln( "Non-diagonal: ", numberNonZerosAddedInStrictlyUT );
  }

  // resulting sparse domain
  var sparseD : sparse subdomain(D) dmapped CS();

  // if adding more than diagonals...
  if numberNonZerosAddedInStrictlyUT > 0 {

    var sDRandomDom : domain(1) = {1..#numberNonZerosInFullUTDomain-N};
    var sDRandom : [sDRandomDom] D.rank*D.idxType;

    // TODO figure out effecient way to add small number of non-zeros

    forall i in D.dim(1).low..D.dim(1).high-1 {

      const delta = (D.dim(1).high-1 - i);
      const positionOffset = ( delta*delta + delta ) / 2;
      const position = positionOffset;
      const colRange = i+1..D.dim(1).high;

      if enableRuntimeDebugging && debugCreateSparseUTDomain then writeln("filling ", i, " x ", colRange);
      for j in colRange {
        sDRandom[ position + (j - i) ] = (i,j);
      }
    }

    shuffle( sDRandom, seed );
    sparseD.bulkAdd( sDRandom[1..#numberNonZerosAddedInStrictlyUT] );
  }

  // Diagonal indices
  var sDDiag : [D.dim(1)] D.rank*D.idxType;
  forall i in D.dim(1) {
    sDDiag[i] = (i,i);
  }
  sparseD.bulkAdd( sDDiag );

  if enableRuntimeDebugging && debugCreateSparseUTDomain then writeln( "there are ", sparseD.size, " non zeros, for density of ", sparseD.size / ( N*N : real ) );

  if sparseD.size != numberNonZerosAddedInUT then halt("Created a domain with unexpected number of non-zero indices. Created %n, expected %n".format(sparseD.size, numberNonZerosAddedInUT));

  return sparseD;
}

proc checkIsUperTriangularDomain( D : domain ) : bool
where D.rank == 2 && isSparseDom( D )
{
  var isUT = true;
  for (i,j) in D {
    isUT = isUT && (i <= j);
    if !isUT then break;
  }
  return isUT;
}

proc prettyPrintSparse( M : [?D] ?T, printIRV : bool = false, separateElements : bool = true )
where D.rank == 2
{
  const padding = max reduce ( [i in M] (i : string).length );
  const formatString = "%%%ns%s".format( padding, if separateElements then " " else "" );
  const blankList = [i in 1..#padding+if separateElements then 1 else 0 ] " ";
  const blankString = "".join( blankList );

  for i in D.dim(1){
    for j in D.dim(2){
      if printIRV || D.member((i,j))
        then writef( formatString, M[i,j] : string );
        else write( blankString );
    }
    writeln();
  }
}

// Number of rows and columns (square matrix)
config const N = 5;

// Density of matrix with only diagonal.
const minDensity : real = 1.0/N;
// Density of matrix with UT region totally filled
const maxDensity : real = (N+1.0)/(2.0*N);

// density in addition to minimum (for ease of use)
// density parameter will be calculated from this.
config const additionalDensity : real = 1.0 - minDensity;

config const density : real = min( maxDensity, minDensity + max(0, additionalDensity) );
config type eltType = int;

config const numTasks : int = here.maxTaskPar;

config const silentMode : bool = false;
config const printStages : bool = !silentMode;
config const printPerfStats : bool = false;
config const printMatrices : bool = false;
config const printNonZeros : bool = false;
config const printPermutations: bool = false;
config const padPrintedMatrixElements = true;
config const seed : int = SeedGenerator.oddCurrentTime;

enum ToposortImplementation { Serial, Parallel };
config const implementation : ToposortImplementation = ToposortImplementation.Parallel;

proc main(){
  if density < minDensity then halt("Specified density (%n) is less than min density (%n) for N (%n)".format( density, minDensity, N) );
  if density > maxDensity then halt("Specified density (%n) is greater than max density (%n) for N (%n)".format( density, maxDensity, N) );

  if !silentMode then writeln( "Number of tasks: %n\nN: %n\nSpecified density: %dr%%".format(numTasks, N, density * 100.0 ) );

  // create upper triangular matrix
  if !silentMode then writeln("Creating sparse upper triangluar domain");
  const D : domain(2) = {1..#N,1..#N};
  const sparseD = createSparseUpperTriangluarDomain( D, density, seed );

  if !silentMode then writeln( "Actual Density: density: %dr%%\nTotal Number NonZeros: %n".format((sparseD.size / (1.0*N*N))*100, sparseD.size) );

  var permutationMap = createRandomPermutationMap( sparseD, seed );
  if printPermutations then writeln("Permutation Map:\n", permutationMap);

  if !silentMode then writeln("Permuting upper triangluar domain");
  var permutedSparseD = permutationMap.permuteDomain( sparseD );

  var topoResult : TopoSortResult(sparseD.idxType);

  select implementation {
    when ToposortImplementation.Serial {
      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Serial implementation.");
      topoResult = toposortSerial( permutedSparseD );
    }
    when ToposortImplementation.Parallel {
      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Parallel implementation.");
      topoResult = toposortParallel( permutedSparseD, numTasks );
    }
    otherwise {
      writeln( "Unknown implementation: ", implementation );
      exit(-1);
    }
  }

  var solvedMap = topoResult.permutationMap;

  if printPerfStats {
    writeln( "Benchmark timers:");
    for timerName in topoResult.timerDom {
      writeln(timerName, ": ", topoResult.timers[timerName].elapsed() );
    }
    writeln( "Rows/second: ", (N/topoResult.timers["whole"].elapsed()) );
  }

  if printPermutations then writeln( "Solved permutation map:\n", solvedMap );

  var solvedPermutedPermutedSparseD = solvedMap.permuteDomain( permutedSparseD );

  if printMatrices {
    var M : [sparseD] eltType;
    var permutedM : [permutedSparseD] eltType;
    var solvedPermuatedPermutedM : [solvedPermutedPermutedSparseD] eltType;

    if !silentMode then writeln("Filling matrix with values");
    if isNumericType( eltType ) {
      var v = 1;
      for idx in sparseD {
        M[idx] = v : eltType;
        permutedM[ permutationMap[idx] ] = v;
        solvedPermuatedPermutedM[ solvedMap[permutationMap[idx]] ] = v;
        v += 1;
      }
    } else if isStringType( eltType ) {
      M = "X";
      M.irv = "_";
      permutedM = "X";
      permutedM.irv = "_";
      solvedPermuatedPermutedM = "X";
      solvedPermuatedPermutedM.irv = "_";
    }

    writeln( "Upper triangluar matrix:" );
    prettyPrintSparse( M, printIRV = printNonZeros, separateElements = padPrintedMatrixElements );

    writeln( "Permuted upper triangluar matrix:" );
    prettyPrintSparse( permutedM, printIRV = printNonZeros, separateElements = padPrintedMatrixElements );

    writeln( "Solved-permuted permuted upper triangluar matrix:" );
    prettyPrintSparse( solvedPermuatedPermutedM, printIRV = printNonZeros, separateElements = padPrintedMatrixElements );
  }

  var isUpperTriangular = checkIsUperTriangularDomain( solvedPermutedPermutedSparseD );
  if !isUpperTriangular then halt("Solved-permuted permuted upper triangluar domain is not upper triangular!");
}
