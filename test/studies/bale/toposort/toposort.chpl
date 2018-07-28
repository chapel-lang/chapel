use Random;
use Time;
use LayoutCS;
use BlockDist;
use CommDiagnostics;

config param enableRuntimeDebugging = true;
config const debugAll : bool = false;
config const debugTopo = debugAll;
config const debugTopoTasking = debugAll || debugTopo;
config const debugCreateSparseUTDomain = debugAll;
config const debugPermute = debugAll;
config const debugWorkQueue = debugAll;
config const debugLock = debugAll;
config const debugSyncLock = debugLock;
config const debugAtomicLock = debugLock;

config param useDimIter = true;
config param useDimIterRow = useDimIter; // not currently supported on master
config param useDimIterCol = false;

config param useDimIterDistributed = false; //SparseBlockDom.dimIter not supported on any rank
config param useDimIterRowDistributed = useDimIterDistributed;
config param useDimIterColDistributed = useDimIterDistributed;

record SyncLock {
  var lock$ : sync bool;

  proc init(){
    this.complete();
    this.lock$.writeEF(true);
  }

  proc lock(){
    if debugSyncLock then writeln("Locking");
    lock$.readFE();
    if debugSyncLock then writeln("Locked");
  }

  proc unlock(){
    if enableRuntimeDebugging && debugSyncLock then writeln("Unlocking");
    lock$.writeEF(true);
    if enableRuntimeDebugging && debugSyncLock then writeln("Unlocked");
  }

  proc forceUnlock(){
    lock$.writeXF(true);
  }
}

record AtomicLock {
  var lock$: atomic bool;

  proc init(){
    this.complete();
    lock$.clear();
  }

  proc lock(){
    while lock$.testAndSet() do chpl_task_yield();
  }

  proc unlock(){
    lock$.clear();
  }
}

class Vector {
  type eltType;
  var listDomain : domain(1);
  var listArray : [listDomain] eltType;
  var size : int;
  var factor : int;

  proc init( type eltType, factor : int = 2, initialCapacity = 10 ){
    this.eltType = eltType;
    assert( factor > 1, "Factor must be > 1");
    this.listDomain = { 1..#initialCapacity };
    this.size = 0;
    this.factor = factor;
  }

  proc init( that : Vector(?eltType) ){
    this.eltType = eltType;
    this.listDomain = that.listDomain;
    this.listArray = that.listArray;
    this.size = that.size;
    this.factor = that.factor;
  }

  proc push_back( value : eltType ){
    if this.size + 1 > this.listDomain.size {
      this.listDomain = {1..#( this.factor * this.listDomain.size )};
    }
    this.listArray[size+1] = value;
    this.size += 1;
  }

  proc push_back( values : Vector(eltType) ){
    while this.size + values.size > this.listDomain.size {
      this.listDomain = {1..#( this.factor * this.listDomain.size )};
    }
    this.listArray[size+1..#values.size] = values.listArray;
    this.size += values.size;
  }

  proc push_back( values : list(eltType) ){
    for value in values {
      this.push_back( value );
    }
  }

  proc clear( ){
    this.size = 0;
  }

  proc compact(){
    this.listDomain = {1..#this.size};
  }
}

class ParallelWorkQueue {
  type eltType;
  type lockType;
  var lock : lockType;
  var queue : Vector(eltType);

  var terminated : atomic bool;
  const terminatedRetries : int;

  proc init( type eltType, type lockType = SyncLock, retries : int = 5 ){
    this.eltType = eltType;
    this.lockType = lockType;
    this.queue = new Vector( eltType );
    this.complete();

    terminated.write(false);
  }

  proc add( value : eltType ){
    this.lock.lock();
    this.queue.push_back( value );
    this.lock.unlock();
  }

  proc add( values : Vector(eltType) ){
    this.lock.lock();
    this.queue.push_back( values );
    this.lock.unlock();
  }

  proc add( values : list(eltType) ){
    this.lock.lock();
    this.queue.push_back( values );
    this.lock.unlock();
  }

  iter these(param tag : iterKind, maxTasks = here.maxTaskPar) : eltType
  where tag == iterKind.standalone {
    // reset termination
    this.terminated.write(false);

    var continueLooping = !this.terminated.read();
    var countdown = this.terminatedRetries;

    // main yield loop.
    while continueLooping {
      var terminated = this.terminated.read();
      continueLooping = !terminated || countdown > 0;
      if terminated then countdown -= 1;

      /* syncLock.lock(); */
      var unlockedQueue = new Vector(eltType);
      // get current chunk of work by quickly swaping out the unlocked queue
      // with the instance queue
      this.lock.lock();
      unlockedQueue <=> this.queue;
      this.lock.unlock();

      // yield all in local chunk
      if unlockedQueue.size > 0 {
        unlockedQueue.compact();
        forall work in unlockedQueue.listArray._value.these( tasksPerLocale = maxTasks ) {
          yield work;
        }
      } else  {
        chpl_task_yield();
      }

      delete unlockedQueue;
    }

    // clear terminated flag
    this.terminated.write(false);
  }

  proc terminate(){
    this.terminated.write(true);
  }
}

class DistributedWorkQueue {
  type eltType;
  type lockType;

  var localesDomain : domain(1) = {1..0};
  var locales : [localesDomain] locale;

  var pid = -1;

  pragma "no doc"
  inline proc _value {
    if pid == -1 then halt("DistributedWorkQueue is uninitialized.");
    return chpl_getPrivatizedCopy(LocalDistributedWorkQueue(eltType,lockType), pid);
  }

  forwarding _value;

  proc init( type eltType, targetLocales : [] locale, type lockType = AtomicLock ){
    this.eltType = eltType;
    this.lockType = lockType;

    this.localesDomain = {0..#targetLocales.domain.size};

    this.complete();
    this.pid = (new LocalDistributedWorkQueue(eltType, lockType, targetLocales)).pid;
  }

  proc deinit(){ }

  pragma "fn returns iterator"
  inline proc these(param tag ) where (tag == iterKind.standalone)
    && __primitive("method call resolves", _value, "these", tag=tag){
    var maxTasks : [0..#Locales.size] int;
    forall onLocale in Locales {
      maxTasks[ onLocale.id ] = onLocale.maxTaskPar;
    }
    return _value.these(tag=tag, maxTasks);
  }

  pragma "fn returns iterator"
  inline proc these(param tag, maxTasksPerLocale : [?d] int ) where (tag == iterKind.standalone)
    && __primitive("method call resolves", _value, "these", tag=tag){
    return _value.these(tag=tag, maxTasksPerLocale);
  }
}

class LocalDistributedWorkQueue {
  type eltType;
  type lockType;

  const localeDomain : domain(1);
  const localeArray : [localeDomain] locale;

  var queue : Vector(eltType);
  var lock : lockType;
  var terminated : atomic bool;
  const terminatedRetries : int;

  var pid = -1;

  proc init( type eltType, type lockType, localeArray : [?localeDomain] locale, retries : int = 5 ){
    this.eltType = eltType;
    this.lockType = lockType;
    this.localeDomain = {0..#localeDomain.size};
    this.localeArray = reshape( localeArray, {0..#localeDomain.size} );
    this.queue = new Vector(eltType);
    this.terminatedRetries = retries;

    this.complete();

    terminated.write(false);
    this.pid = _newPrivatizedClass(this);
  }

  proc init( that : LocalDistributedWorkQueue(?eltType, ?lockType), pid : int) {
    this.eltType = eltType;
    this.lockType = lockType;
    this.localeDomain = that.localeDomain;
    this.localeArray = that.localeArray;
    this.queue = new Vector( that.queue );
    this.terminatedRetries = that.terminatedRetries;
    this.pid = pid;

    this.complete();

    this.terminated.write(false);
  }

  proc deinit(){ }

  proc dsiGetPrivatizeData() {
    return pid;
  }

  proc dsiPrivatize(pid) {
    return new LocalDistributedWorkQueue(this, pid);
  }

  inline proc getPrivatizedThis {
    return chpl_getPrivatizedCopy(this.type, this.pid);
  }

  proc add( value : eltType, owner : locale ) {
    on owner {
      var instance = getPrivatizedThis;
      instance.lock.lock();
      instance.queue.push_back( value );
      instance.lock.unlock();
    }
  }

  pragma "fn returns iterator"
  inline proc these(param tag) where (tag == iterKind.standalone) {
    var maxTasks : [0..#Locales.size] int;
    forall onLocale in Locales {
      maxTasks[ onLocale.id ] = onLocale.maxTaskPar;
    }
    return this.these(tag=tag, maxTasks);
  }

  iter these(param tag : iterKind, maxTasksPerLocale : [] int) : eltType
  where tag == iterKind.standalone {
    coforall onLocale in localeArray do on onLocale {

      const maxTasks = maxTasksPerLocale[onLocale.id];
      // get local copy
      var instance = getPrivatizedThis;

      // reset termination
      instance.terminated.write(false);

      var continueLooping = !this.terminated.read();
      var countdown = this.terminatedRetries;

      // main yield loop.
      while continueLooping {
        var terminated = this.terminated.read();
        continueLooping = !terminated || countdown > 0;
        if terminated then countdown -= 1;

        var unlockedQueue = new Vector(eltType);
        // get current chunk of work by quickly swaping out the unlocked queue
        // with the instance queue
        instance.lock.lock();
        unlockedQueue <=> instance.queue;
        instance.lock.unlock();

        // yield all in local chunk
        if unlockedQueue.size > 0 {
          unlockedQueue.compact();
          forall work in unlockedQueue.listArray._value.these( tasksPerLocale = maxTasks ) {
            yield work;
          }
        } else  {
          chpl_task_yield();
        }

        delete unlockedQueue;
      }

    }

    // clear terminated flag
    coforall onLocale in localeArray do on onLocale {
      var instance = getPrivatizedThis;
      instance.terminated.write(false);
    }
  }

  proc terminate(){
    coforall onLocale in this.localeArray {
      on onLocale {
        var instance = getPrivatizedThis;
        instance.terminated.write(true);
      }
    }
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
  var sparseD : sparse subdomain(D) dmapped CS(compressRows=false);

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

proc toposortSerial( D : domain )
where D.rank == 2
{
  var result = new TopoSortResult(D.idxType);
  result.timers["whole"].start();

  const rows = D.dim(1);
  const columns = D.dim(2);
  const numDiagonals = min( rows.size, columns.size );

  var rowMap : [rows] D.idxType = [i in rows] -1;
  var columnMap : [columns] D.idxType = [i in columns] -1;

  var rowSum : [rows] int;
  var rowCount : [rows] int;
  var workQueue : list(D.idxType);

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  for row in rows {
    if enableRuntimeDebugging && debugTopo then writeln( "initializing row ", row );
    if useDimIterCol {
     // compilerWarning("iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        rowCount[row] += 1;
        rowSum[row] += col;
      }
    } else {
      // compilerWarning("iterating over columns in init with dim");
      for col in columns {
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
    rowMap[swapRow] = rows.high - diagonalPosition;
    columnMap[swapColumn] = columns.high - diagonalPosition;
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
      for row in rows {
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

proc toposortParallel( D : domain, numTasks : int = here.maxTaskPar )
where D.rank == 2
{
  if numTasks < 1 then halt("Must run with numTaks >= 1");

  var result = new TopoSortResult(D.idxType);
  result.timers["whole"].start();

  const rows = D.dim(1);
  const columns = D.dim(2);
  const numDiagonals = min( rows.size, columns.size );

  var rowMap : [rows] D.idxType = [i in rows] -1;
  var columnMap : [columns] D.idxType = [i in columns] -1;

  var rowSum : [rows] atomic int;
  var rowCount : [rows] atomic int;
  var workQueue = new ParallelWorkQueue(D.idxType);

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  forall row in rows {
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
      for col in columns {
        if D.member((row,col)) {
          count += 1;
          sum += col;
        }
      }
    }

    if count == 1 {
      workQueue.add( row );
    }

    rowCount[row].write( count );
    rowSum[row].write( sum );
  }
  result.timers["initialization"].stop();

  if enableRuntimeDebugging && debugTopo {
    /* writeln( "initial workQueue ", workQueue ); */
    writeln( "initial rowSum    ", rowSum );
    writeln( "initial rowCount  ", rowCount );
  }

  // insert position along diagonal from (N,N)
  var diagonalPosition : atomic int;
  diagonalPosition.write(0);

  result.timers["toposort"].start();

  // For each queued row (and rows that will be queued)...
  forall swapRow in workQueue {
    // The body of this loop executes on the locale where swapRow is queued

    if enableRuntimeDebugging && debugTopo {
      writeln(
        "========================",
        /* "\nworkQueue ", workQueue, */
        "\nrowSum    ", rowSum,
        "\nrowCount  ", rowCount,
        "\nrowMap    ", rowMap,
        "\ncolumnMap ", columnMap,
        "\n========================"
      );
    }

    // get non-zero column
    var swapColumn = rowSum[swapRow].read();
    // get and infrement localDiagonal
    var localDiagonal = diagonalPosition.fetchAdd(1);
    // permute this row to the diagonal
    rowMap[swapRow] = rows.high - localDiagonal;
    columnMap[swapColumn] = columns.high - localDiagonal;

    // if localDiagonal == numDiagonals - 1 (i.e. diagonalPosition == numDiagonals)
    if localDiagonal == numDiagonals - 1 {
      if enableRuntimeDebugging && debugTopo then writeln("Terminating Loop");
      workQueue.terminate();
    }

    if enableRuntimeDebugging && debugTopo then writeln( "Swaping ", (swapRow,swapColumn), " -> ", (rowMap[swapRow], columnMap[swapColumn]) );

    // foreach row along the swapped column who has a nonzero at (row, swapColumn)
    // remove swapColumn from rowSum and reduce rowCount
    if useDimIterRow {
      // compilerWarning("iterating over rows in kernel with dimIter");
      for row in D.dimIter(1,swapColumn) {
        var previousRowCount = rowCount[row].fetchSub( 1 );
        rowSum[row].sub( swapColumn );
        // if previousRowCount = 2 (ie rowCount[row] == 1)
        if previousRowCount == 2 {
          if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
          workQueue.add( row );
        }
      }
    } else {
      // compilerWarning("iterating over rows in kernel with dim");
      for row in rows {
        if D.member((row, swapColumn)) {
          var previousRowCount = rowCount[row].fetchSub( 1 );
          rowSum[row].sub( swapColumn );
          // if previousRowCount = 2 (ie rowCount[row] == 1)
          if previousRowCount == 2 {
            if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
            workQueue.add( row );
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

proc toposortDistributed( D : domain )
where D.rank == 2
{
  var maxTasksPerLocale : [0..#Locales.size] int;
  for onLocale in Locales {
    maxTasksPerLocale[onLocale.id] = onLocale.maxTaskPar;
  }
  return toposortDistributed( D, maxTasksPerLocale );
}

proc toposortDistributed( D : domain, maxTasksPerLocale : [] int )
where D.rank == 2
{
  if (min reduce maxTasksPerLocale) < 1 then halt("Must run with numTasks >= 1");

  var result = new TopoSortResult(D.idxType);
  result.timers["whole"].start();

  const rows = D.dim(1);
  const columns = D.dim(2);
  const numDiagonals = min( rows.size, columns.size );
  const minCol = columns.low;

  var rowMap : [rows] D.idxType = [i in rows] -1;
  var columnMap : [columns] D.idxType = [i in columns] -1;

  var rowSum : [rows] atomic int;
  var rowCount : [rows] atomic int;
  var workQueue = new DistributedWorkQueue(D.idxType, Locales);

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  forall row in rows {
    // Accumulate task locally, then write at end.
    var count = 0;
    var sum = 0;

    if enableRuntimeDebugging && debugTopo then writeln( "initializing row ", row );
    if useDimIterColDistributed {
     // compilerWarning("iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        count += 1;
        sum += col;
      }
    } else {
      // compilerWarning("iterating over columns in init with dim");
      for col in columns {
        if D.member((row,col)) {
          count += 1;
          sum += col;
        }
      }
    }

    if count == 1 {
      workQueue.add( row, D.dist.dsiIndexToLocale( (row,minCol) ) );
    }

    rowCount[row].write( count );
    rowSum[row].write( sum );
  }
  result.timers["initialization"].stop();

  if enableRuntimeDebugging && debugTopo {
    writeln( "initial workQueue ", workQueue );
    writeln( "initial rowSum    ", rowSum );
    writeln( "initial rowCount  ", rowCount );
  }

  // insert position along diagonal from (N,N)
  var diagonalPosition : atomic int;
  diagonalPosition.write(0);

  result.timers["toposort"].start();

  // For each queued row (and rows that will be queued)...
  // TODO make workQueue.these() accept maxTasksPerLocale
  forall swapRow in workQueue {
    // The body of this loop executes on the locale where swapRow is queued

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

    // get non-zero column
    var swapColumn = rowSum[swapRow].read();
    // get and infrement localDiagonal
    var localDiagonal = diagonalPosition.fetchAdd(1);
    // permute this row to the diagonal
    rowMap[swapRow] = rows.high - localDiagonal;
    columnMap[swapColumn] = columns.high - localDiagonal;

    // if localDiagonal == numDiagonals - 1 (i.e. diagonalPosition == numDiagonals)
    if localDiagonal == numDiagonals - 1 {
      if enableRuntimeDebugging && debugTopo then writeln("Terminating Loop");
      workQueue.terminate();
    }

    if enableRuntimeDebugging && debugTopo then writeln( "Swaping ", (swapRow,swapColumn), " -> ", (rowMap[swapRow], columnMap[swapColumn]) );

    // foreach row along the swapped column who has a nonzero at (row, swapColumn)
    // remove swapColumn from rowSum and reduce rowCount
    // NOTE: dimIter is not supported on any dimension on SparseBlockDom
    if useDimIterRowDistributed {
      // compilerWarning("iterating over rows in kernel with dimIter");
      for row in D.dimIter(1,swapColumn) {
        var previousRowCount = rowCount[row].fetchSub( 1 );
        rowSum[row].sub( swapColumn );
        // if previousRowCount = 2 (ie rowCount[row] == 1)
        if previousRowCount == 2 {
          if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
          workQueue.add( row, D.dist.dsiIndexToLocale( (row,minCol) ) );
        }
      }
    } else {
      // compilerWarning("iterating over rows in kernel with dim");
      for row in rows {
        if D.member((row, swapColumn)) {
          var previousRowCount = rowCount[row].fetchSub( 1 );
          rowSum[row].sub( swapColumn );
          // if previousRowCount = 2 (ie rowCount[row] == 1)
          if previousRowCount == 2 {
            if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
            workQueue.add( row, D.dist.dsiIndexToLocale( (row,minCol) ) );
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
config type eltType = string;

config const numTasks : int = here.maxTaskPar;

config const silentMode : bool = false;
config const printStages : bool = !silentMode;
config const printPerfStats : bool = false;
config const printMatrices : bool = false;
config const printNonZeros : bool = false;
config const printPermutations: bool = false;
config const padPrintedMatrixElements = true;
config const seed : int = SeedGenerator.oddCurrentTime;

enum ToposortImplementation { Serial, Parallel, Distributed };
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
    when ToposortImplementation.Distributed {
      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Distributed implementation.");
      var distributedD : D.type dmapped Block(D, targetLocales=reshape(Locales, {Locales.domain.dim(1),1..#1}) ) = D;

      var distributedPermutedSparseD : sparse subdomain(distributedD);
      // TODO make faster...
      for i in permutedSparseD {
        distributedPermutedSparseD += i;
      }
      topoResult = toposortDistributed( distributedPermutedSparseD );
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
