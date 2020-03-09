use Random;
use Time;
use LayoutCS;
use BlockDist;
use CommDiagnostics;
use Sort;
use LinkedLists;
use IO;

config param enableRuntimeDebugging = true;
config const debugAll : bool = false;
config const debugTopo = debugAll;
config const debugTopoTasking = debugAll || debugTopo;
config const debugCreateDomain = debugAll;
config const debugPermute = debugAll;
config const debugWorkQueue = debugAll;
config const debugLock = debugAll;
config const debugSyncLock = debugLock;
config const debugAtomicLock = debugLock;

config param warnDimIterMethod = false;

config param useDimIter = true;
config param useDimIterRow = useDimIter; // supported for CSC domains
config param useDimIterCol = false; // not currently supported on master

config param useDimIterDistributed = false; //SparseBlockDom.dimIter not supported on any rank
config param useDimIterRowDistributed = useDimIterDistributed;
config param useDimIterColDistributed = useDimIterDistributed;

class SyncLock {
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

  proc isLocked() : bool {
    return this.lock$.isFull;
  }
}

class AtomicLock {
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

  proc isLocked() : bool {
    return this.lock$.read();
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

  proc push_back( values : LinkedList(eltType) ){
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
  var queue : unmanaged Vector(eltType);

  var terminated : atomic bool;
  const terminatedRetries : int;

  proc init( type eltType, type lockType = unmanaged SyncLock, retries : int = 5 ){
    if isClassType(lockType) && !isUnmanagedClassType(lockType) then
      compilerError("Expected unmanaged lockType");

    this.eltType = eltType;
    this.lockType = lockType;
    this.lock = new lockType();
    this.queue = new unmanaged Vector( eltType );
    this.complete();

    terminated.write(false);
  }

  proc deinit(){
    delete this.lock;
    delete this.queue;
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

  proc add( values : LinkedList(eltType) ){
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
      var unlockedQueue = new unmanaged Vector(eltType);
      // get current chunk of work by quickly swaping out the unlocked queue
      // with the instance queue
      this.lock.lock();
      unlockedQueue <=> this.queue;
      this.lock.unlock();

      // yield all in local chunk
      if unlockedQueue.size > 0 {
        unlockedQueue.compact();
        forall work in unlockedQueue.listArray.these( tasksPerLocale = maxTasks ) {
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

//var localesDomain : domain(1) = {1..0};
//var locales : [localesDomain] locale;

  var localInstance : unmanaged LocalDistributedWorkQueue(eltType, lockType);
  var pid = -1;

  pragma "no doc"
  inline proc _value {
    if pid == -1 then halt("DistributedWorkQueue is uninitialized.");
    return chpl_getPrivatizedCopy(unmanaged LocalDistributedWorkQueue(eltType,lockType), pid);
  }

  forwarding _value;

  proc init( type eltType, targetLocales : [] locale, type lockType = unmanaged AtomicLock ){
    this.eltType = eltType;
    this.lockType = lockType;

//  this.localesDomain = {0..#targetLocales.domain.size};
//  this.locales = reshape(targetLocales, this.localesDomain);

    this.localInstance = new unmanaged LocalDistributedWorkQueue(eltType, lockType, targetLocales);
    this.pid = this.localInstance.pid;
    this.complete();
  }

  proc deinit(){
    delete this.localInstance;
  }
}

class LocalDistributedWorkQueue {
  type eltType;
  type lockType;

  const localeDomain : domain(1);
  const localeArray : [localeDomain] locale;

  var lock : lockType;
  var queue : unmanaged Vector(eltType);
  var terminated : atomic bool;
  const terminatedRetries : int;

  var pid = -1;

  proc init( type eltType, type lockType, localeArray : [?localeDomain] locale, retries : int = 5 ){
    if isClassType(lockType) && !isUnmanagedClassType(lockType) then
      compilerError("Expected unmanaged lockType");

    this.eltType = eltType;
    this.lockType = lockType;
    this.localeDomain = {0..#localeDomain.size};
    this.localeArray = reshape( localeArray, {0..#localeDomain.size} );
    this.lock = new lockType();
    this.queue = new unmanaged Vector(eltType);
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
    this.lock = new lockType();
    this.queue = new unmanaged Vector( that.queue );
    this.terminatedRetries = that.terminatedRetries;
    this.pid = pid;

    this.complete();

    this.lock.lock$.write( that.lock.isLocked() );
    this.terminated.write(that.terminated.read());
  }

  proc deinit(){
    delete this.lock;
    delete this.queue;
  }

  proc dsiGetPrivatizeData() {
    return pid;
  }

  proc dsiPrivatize(pid) {
    return new unmanaged LocalDistributedWorkQueue(this, pid);
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

        var unlockedQueue = new unmanaged Vector(eltType);
        // get current chunk of work by quickly swaping out the unlocked queue
        // with the instance queue
        instance.lock.lock();
        unlockedQueue <=> instance.queue;
        instance.lock.unlock();

        // yield all in local chunk
        if unlockedQueue.size > 0 {
          unlockedQueue.compact();
          forall work in unlockedQueue.listArray.these( tasksPerLocale = maxTasks ) {
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

  override proc writeThis( f ){
    const maxVal = max( (max reduce rowMap), (max reduce columnMap) ) : string;
    const minVal = min( (min reduce rowMap), (min reduce columnMap) ) : string;
    const padding = max( maxVal.size, minVal.size );
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

  proc permuateIndexList( array : [?D] rank*idxType ) : [D] rank*idxType {
    var retArray : [array.domain] array.eltType;
    forall i in retArray.domain {
      retArray[i] = this( array[i] );
    }
    return retArray;
  }
}

class TopoSortResult {
  type idxType;
  var permutationMap : shared PermutationMap(idxType)?;
  var timerDom : domain(string);
  var timers : [timerDom] Timer;

  proc init(type idxType){
    this.idxType = idxType;
    this.permutationMap = nil;
    this.timerDom = {"whole","initialization","toposort"};
  }
}

proc createRandomPermutationMap( D : domain, seed : int ) : shared PermutationMap(D.idxType)
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
  return new shared PermutationMap( rowMap, columnMap );
}

proc createSparseUpperTriangluarIndexList(
  D : domain(2),
  density : real,
  seed : int,
  fillModeDensity : real
) {
  // Must be square matrix, uniformly dimensioned dense domain
  if D.dim(1).size != D.dim(2).size then halt("Domain provided to createSparseUpperTriangluarDomain is not square.");
  if (D.dim(1).low != D.dim(2).low) || (D.dim(1).high != D.dim(2).high) then halt("Domain provided to createSparseUpperTriangluarDomain does not have equivalent ranges.");
  const N = D.dim(1).size;
  const low = D.dim(1).low;
  const high = D.dim(1).high;
  const minDensity : real = 1.0/N;
  const maxDensity : real = (N+1.0)/(2.0*N);

  // assert we have been given a legal density;
  if density < minDensity then halt( "Specified density (%n) is less than minimum density (%n) for N (%n)".format( density, minDensity, N));
  if density > maxDensity then halt( "Specified density (%n) is greater than maximum density (%n) for N (%n)".format( density, maxDensity, N));

  // Maximum number of non-zero elements in 100% dense UT matrix
  const maxNumberNonZerosInFullUTDomain : int = (( N * N + N)/2.0) : int;
  // Maximum number of non-zeros elements in 100% desn UT matrix
  const maxNumberNonZerosStrictlyInUT : int = maxNumberNonZerosInFullUTDomain - N;
  // number of elements added
  const numberNonZerosAddedInUT : int = max( N, floor( N*N*density ) ) : int;
  // number of non-diagonal elements added
  const numberNonZerosAddedInStrictlyUT : int = numberNonZerosAddedInUT - N;

  if enableRuntimeDebugging && debugCreateDomain {
    writeln( "Dense: ", maxNumberNonZerosInFullUTDomain );
    writeln( "Added: ", numberNonZerosAddedInUT );
    writeln( "Non-diagonal added: ", numberNonZerosAddedInStrictlyUT );
  }

  // Resulting list of indices
  var sparseD : [1..#numberNonZerosAddedInUT] D.rank*D.idxType;

  // if adding non-diagonals
  if numberNonZerosAddedInStrictlyUT > 0 {
    // if dense enough to warrant a dense fill and remove methods
    // Note: this is pretty effecient, even for low densities
    if density >= fillModeDensity {
      var sDRandomDom : domain(1) = {1..#maxNumberNonZerosStrictlyInUT};
      var sDRandom : [sDRandomDom] D.rank*D.idxType;

      // foreach row
      forall row in low..high-1 {
        // Inital position in sDRandom is Sum(N-1) - Sum( (N-1) - (row-1) ) + 1
        var i = (-((N-row)*(N-row)) - N + row) / 2 + ((N-1)*(N-1) + N - 1)/2 + 1;
        for column in row+1..high {
          sDRandom[ i ] = (row,column);
          i += 1; // next position
        }
      }

      // if not maximum density, shuffle, add subset
      if numberNonZerosAddedInStrictlyUT < maxNumberNonZerosStrictlyInUT {
        shuffle( sDRandom, seed );
        /* sparseD.bulkAdd( sDRandom[1..#numberNonZerosAddedInStrictlyUT] ); */
        sparseD[N+1..#numberNonZerosAddedInStrictlyUT] = sDRandom[1..#numberNonZerosAddedInStrictlyUT];
      } else {
        /* sparseD.bulkAdd( sDRandom ); */
        sparseD[N+1..#numberNonZerosAddedInStrictlyUT] = sDRandom;
      }
      // add to returned sparse domain
    }
    // If *very* sparse, use insertion fill (not effecient even at relatively low densities)
    else {
      var random : RandomStream(D.idxType) = new owned RandomStream(D.idxType, seed);

      // number of non-zero columns in each row in the strict UT region
      var rowCount : [low..high-1] int;
      var totalAdded = 0;
      while totalAdded < numberNonZerosAddedInStrictlyUT {
        // get random row
        var row = (abs( random.getNext() ) % (high-1)) + 1;
        // if row is not filled, add
        if rowCount[row] < high-row{
          rowCount[row] += 1;
          totalAdded += 1;
        }
      }

      // list of indices in strictly UT region
      var sDRandomDom : domain(1) = {1..#numberNonZerosAddedInStrictlyUT};
      var sDRandom : [sDRandomDom] D.rank*D.idxType;

      // array of start index into sDRandom for each row.
      // row=high is unused, can be used for asserting( i < startOffset[row+1] )
      var startOffset : [low..high] int;
      for row in startOffset.domain {
        if row == low {
           startOffset[row] = sDRandomDom.low; // base case
        } else {
          startOffset[row] = startOffset[row-1] + rowCount[row-1];
        }
      }

      // foreach row...
      forall row in low..high-1 {
        // create a new local random number generator
        // note: seed is still deterministic. Should get same behavior regardless
        // of tasking (including number of tasks and scheduling)
        var localRandom : RandomStream(D.idxType) = new owned RandomStream(D.idxType, (seed ^ (row << 1) | 1) );
        // our index into sDRandomDom
        var i = startOffset[row];
        // set of indices we already have
        var bagOfIndices : domain(D.idxType);
        while rowCount[row] > 0 {
          // random column in range row+1 .. high
          var column = (abs( localRandom.getNext() ) % (high-row)) + row+1;
          // if not already in our index set, add it
          if !bagOfIndices.contains( column ) {
            bagOfIndices += column; // add to set
            rowCount[row] -= 1; // decrement count
            sDRandom[i] = (row,column); // add to index array
            i += 1; // increment sDRandom index to next position
          }
        }
      }
      // add to returned sparse domain
      /* sparseD.bulkAdd( sDRandom ); */
      sparseD[N+1..#numberNonZerosAddedInStrictlyUT] = sDRandom;
    }
  }

  // Diagonal indices
  forall i in D.dim(1) {
    sparseD[i] = (i,i);
  }

  if enableRuntimeDebugging && debugCreateDomain then writeln( "there are ", sparseD.size, " non zeros, for density of ", sparseD.size / ( N*N : real ) );

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

proc checkIsUperTriangularIndexList( array : [?D] 2*int ) : bool
{
  var isUT = true;
  for (i,j) in array {
    isUT = isUT && (i <= j);
    if !isUT then break;
  }
  return isUT;
}

proc prettyPrintSparse( M : [?D] ?T, printIRV : bool = false, separateElements : bool = true )
where D.rank == 2
{
  const padding = max reduce ( [i in M] (i : string).size );
  const formatString = "%%%ns%s".format( padding, if separateElements then " " else "" );
  const blankList = [i in 1..#padding+if separateElements then 1 else 0 ] " ";
  const blankString = "".join( blankList );

  for i in D.dim(1){
    for j in D.dim(2){
      if printIRV || D.contains((i,j))
        then writef( formatString, M[i,j] : string );
        else write( blankString );
    }
    writeln();
  }
}

proc toposortSerial( D : domain ) : shared TopoSortResult(D.idxType)
where D.rank == 2
{
  var result = new shared TopoSortResult(D.idxType);
  result.timers["whole"].start();

  const rows = D.dim(1);
  const columns = D.dim(2);
  const numDiagonals = min( rows.size, columns.size );

  var rowMap : [rows] D.idxType = [i in rows] -1;
  var columnMap : [columns] D.idxType = [i in columns] -1;

  var rowSum : [rows] int;
  var rowCount : [rows] int;
  var workQueue : LinkedList(D.idxType);

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  for row in rows {
    if enableRuntimeDebugging && debugTopo then writeln( "initializing row ", row );
    if useDimIterCol {
     if warnDimIterMethod then compilerWarning("toposortSerial.init iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        rowCount[row] += 1;
        rowSum[row] += col;
      }
    } else {
      if warnDimIterMethod then compilerWarning("toposortSerial.init iterating over columns in init with dim");
      for col in columns {
        if D.contains((row,col)) {
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
      if warnDimIterMethod then compilerWarning("toposortSerial.toposort iterating over rows in kernel with dimIter");
      for row in D.dimIter(1,swapColumn) {
        rowCount[row] -= 1;
        rowSum[row] -= swapColumn;
        if rowCount[row] == 1 {
          if enableRuntimeDebugging && debugTopo then writeln( "Queueing ", row);
          workQueue.push_back( row );
        }
      }
    } else {
      if warnDimIterMethod then compilerWarning("toposortSerial.toposort iterating over rows in kernel with dim");
      for row in rows {
        if D.contains((row, swapColumn)) {
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

proc toposortParallel( D : domain, numTasks : int = here.maxTaskPar ) : shared TopoSortResult(D.idxType)
where D.rank == 2
{
  if numTasks < 1 then halt("Must run with numTaks >= 1");

  var result = new shared TopoSortResult(D.idxType);
  result.timers["whole"].start();

  const rows = D.dim(1);
  const columns = D.dim(2);
  const numDiagonals = min( rows.size, columns.size );

  var rowMap : [rows] D.idxType = [i in rows] -1;
  var columnMap : [columns] D.idxType = [i in columns] -1;

  var rowSum : [rows] atomic int;
  var rowCount : [rows] atomic int;
  var workQueue = new owned ParallelWorkQueue(D.idxType);

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  forall row in rows {
    // Accumulate task locally, then write at end.
    var count = 0;
    var sum = 0;

    if enableRuntimeDebugging && debugTopo then writeln( "initializing row ", row );
    if useDimIterCol {
     if warnDimIterMethod then compilerWarning("toposortParallel.init iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        count += 1;
        sum += col;
      }
    } else {
      if warnDimIterMethod then compilerWarning("toposortParallel.init iterating over columns in init with dim");
      for col in columns {
        if D.contains((row,col)) {
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
      if warnDimIterMethod then compilerWarning("toposortParallel.toposort iterating over rows in kernel with dimIter");
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
      if warnDimIterMethod then compilerWarning("toposortParallel.toposort iterating over rows in kernel with dim");
      for row in rows {
        if D.contains((row, swapColumn)) {
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

proc toposortDistributed( D : domain ) : shared TopoSortResult(D.idxType)
where D.rank == 2
{
  var maxTasksPerLocale : [0..#Locales.size] int;
  for onLocale in Locales {
    maxTasksPerLocale[onLocale.id] = onLocale.maxTaskPar;
  }
  return toposortDistributed( D, maxTasksPerLocale );
}

proc toposortDistributed( D : domain, maxTasksPerLocale : [] int ) : shared TopoSortResult(D.idxType)
where D.rank == 2
{
  if (min reduce maxTasksPerLocale) < 1 then halt("Must run with numTasks >= 1");

  var result = new shared TopoSortResult(D.idxType);
  result.timers["whole"].start();

  const rows = D.dim(1);
  const columns = D.dim(2);
  const numDiagonals = min( rows.size, columns.size );
  const minCol = columns.low;

  var rowMap : [rows] D.idxType = [i in rows] -1;
  var columnMap : [columns] D.idxType = [i in columns] -1;

  var rowSum : [rows] atomic int;
  var rowCount : [rows] atomic int;
  var workQueue = new owned DistributedWorkQueue(D.idxType, Locales);

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  forall row in rows {
    // Accumulate task locally, then write at end.
    var count = 0;
    var sum = 0;

    if enableRuntimeDebugging && debugTopo then writeln( "initializing row ", row );
    if useDimIterColDistributed {
     if warnDimIterMethod then compilerWarning("toposortDistributed.init iterating over columns in init with dimIter");
      for col in D.dimIter(2,row) {
        count += 1;
        sum += col;
      }
    } else {
      if warnDimIterMethod then compilerWarning("toposortDistributed.init iterating over columns in init with dim");
      for col in columns {
        if D.contains((row,col)) {
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
      if warnDimIterMethod then compilerWarning("toposortDistributed.toposort iterating over rows in kernel with dimIter");
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
      if warnDimIterMethod then compilerWarning("toposortDistributed.toposort iterating over rows in kernel with dim");
      for row in rows {
        if D.contains((row, swapColumn)) {
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
// density at and above which createSparseUpperTriangluarDomain uses dense fill
// and below which uses sparse insert
config const defaultFillModeDensity : real = .1;

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
  if density < minDensity then halt("Specified density (%n) is less than min density (%n) for N (%n)".format( density, minDensity, N));
  if density > maxDensity then halt("Specified density (%n) is greater than max density (%n) for N (%n)".format( density, maxDensity, N));

  if defaultFillModeDensity > 1 || defaultFillModeDensity < 0 then halt("defaultFillModeDensity must be in [0.0, 1.0] (is %n)".format(defaultFillModeDensity));

  if !silentMode then writeln( "Number of tasks: %n\nN: %n\nSpecified density: %dr%%".format(numTasks, N, density * 100.0 ) );

  // create upper triangular matrix
  if !silentMode then writeln("Creating sparse upper triangluar domain");
  const D : domain(2) = {1..#N,1..#N};

  const sparseUpperTriangularIndexList = createSparseUpperTriangluarIndexList( D, density, seed, defaultFillModeDensity );

  if !silentMode then writeln( "Actual Density: density: %dr%%\nTotal Number NonZeros: %n".format((sparseUpperTriangularIndexList.size / (1.0*N*N))*100, sparseUpperTriangularIndexList.size) );

  var permutationMap = createRandomPermutationMap( D, seed );
  if printPermutations then writeln("Permutation Map:\n", permutationMap);

  if !silentMode then writeln("Permuting upper triangluar domain");
  var permutedSparseUpperTriangularIndexList = permutationMap.permuateIndexList( sparseUpperTriangularIndexList );

  var topoResult : shared TopoSortResult(D.idxType)?;

  select implementation {
    when ToposortImplementation.Serial {
       if !silentMode then writeln("Converting to CSC domain");

      var dmappedPermutedSparseD : sparse subdomain(D) dmapped CS(compressRows=false);
      dmappedPermutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );

      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Serial implementation.");
      topoResult = toposortSerial( dmappedPermutedSparseD );
    }
    when ToposortImplementation.Parallel {
       if !silentMode then writeln("Converting to CSC domain");

      var dmappedPermutedSparseD : sparse subdomain(D) dmapped CS(compressRows=false);
      dmappedPermutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );

      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Parallel implementation.");
      topoResult = toposortParallel( dmappedPermutedSparseD, numTasks );
    }
    when ToposortImplementation.Distributed {
       if !silentMode then writeln("Converting to Sparse Block domain");
      var distributedD : D.type dmapped Block(D, targetLocales=reshape(Locales, {Locales.domain.dim(1),1..#1}) ) = D;

      var distributedPermutedSparseD : sparse subdomain(distributedD);
      distributedPermutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );

      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Distributed implementation.");
      topoResult = toposortDistributed( distributedPermutedSparseD );
    }
    otherwise {
      writeln( "Unknown implementation: ", implementation );
      exit(-1);
    }
  }

  var solvedMap = topoResult!.permutationMap!;

  if printPerfStats {
    writeln( "Benchmark timers:");
    for timerName in topoResult!.timerDom {
      writeln(timerName, ": ", topoResult!.timers[timerName].elapsed() );
    }
    writeln( "Rows/second: ", (N/topoResult!.timers["whole"].elapsed()) );
  }

  if printPermutations then writeln( "Solved permutation map:\n", solvedMap );

  var solvedPermutedIndexList = solvedMap.permuateIndexList( permutedSparseUpperTriangularIndexList );

  if printMatrices {
    var sparseD : sparse subdomain(D);
    var permutedSparseD : sparse subdomain(D);
    var solvedPermutedPermutedSparseD : sparse subdomain(D);


    sparseD.bulkAdd( sparseUpperTriangularIndexList );
    permutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );
    solvedPermutedPermutedSparseD.bulkAdd( solvedPermutedIndexList );

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

  var isUpperTriangular = checkIsUperTriangularIndexList( solvedPermutedIndexList );
  if !isUpperTriangular then halt("Solved-permuted permuted upper triangluar domain is not upper triangular!");
}
