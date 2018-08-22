use Random;
use Time;
use LayoutCS;
use BlockDist;
use CommDiagnostics;
use Sort;

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

config param warnMethods = false;
config param warnDimIterMethod = warnMethods;
config param warnParallelWorkQueueTheseMethod = warnMethods;

config param useDimIter = true;
config param useDimIterRow = useDimIter; // supported for CSC domains
config param useDimIterCol = false; // not currently supported on master

config param useDimIterDistributed = false; //SparseBlockDom.dimIter not supported on any rank
config param useDimIterRowDistributed = useDimIterDistributed;
config param useDimIterColDistributed = useDimIterDistributed;

// configures number of bytes to pad elements in the ParallelWorkQueueChunkedLoopWaitPadding
// array of queues and locks
// Default value assumes a x86 cache line (64 bytes) - 2 pointers (8 bytes each)
config param ParallelWorkQueueChunkedLoopWaitPaddingBytes : int = 64-(2*8);

/*
Sync variable backed lock.
*/
class SyncLock {
  pragma "no doc"
  var lock$ : sync bool;

  proc init(){
    this.complete();
    this.lock$.writeEF(true);
  }

  /*
  Lock.
  */
  proc lock(){
    lock$.readFE();
  }

  /*
  Unlock.
  */
  proc unlock(){
    lock$.writeEF(true);
  }

  /*
  Non-blocking unlock.
  */
  proc forceUnlock(){
    lock$.writeXF(true);
  }

  /*
  Peak at lock state.
  :returns: true if locked.
  :rtype: bool
  */
  proc isLocked() : bool {
    return this.lock$.isFull;
  }
}

/*
Atomic backed lock.
*/
class AtomicLock {
  pragma "no doc"
  var lock$: atomic bool;

  proc init(){
    this.complete();
    lock$.clear();
  }

  /*
  Lock
  */
  proc lock(){
    while lock$.testAndSet() do chpl_task_yield();
  }

  /*
  Unlock
  */
  proc unlock(){
    lock$.clear();
  }

  /*
  Peak at lock state.
  :returns: true if locked.
  :rtype: bool
  */
  proc isLocked() : bool {
    return this.lock$.read();
  }
}

/*
Very basic push-back queue.
Values are not popped from.
*/
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

  proc init( that : unmanaged Vector(?eltType) ){
    this.eltType = eltType;
    this.listDomain = that.listDomain;
    this.listArray = that.listArray;
    this.size = that.size;
    this.factor = that.factor;
  }

  /*
  Push back a single value.
  :arg value: Value to be pushed onto to end of list.
  :type value: eltType.
  */
  proc push_back( value : eltType ){
    if this.size + 1 > this.listDomain.size {
      this.listDomain = {1..#( this.factor * this.listDomain.size )};
    }
    this.listArray[size+1] = value;
    this.size += 1;
  }

  /*
  Push back all values from another Vector.
  :arg values: Vector of values to copied and pushed onto end of this Vector.
  :type values: unmanaged Vector(eltType).
  */
  proc push_back( values : unmanaged Vector(eltType) ){
    while this.size + values.size > this.listDomain.size {
      this.listDomain = {1..#( this.factor * this.listDomain.size )};
    }
    this.listArray[size+1..#values.size] = values.listArray;
    this.size += values.size;
  }

  /*
  Push back all values from a list.
  :arg values: list of values to copied and pushed onto end of this Vector.
  :type values: list(eltType).
  */
  proc push_back( values : list(eltType) ){
    for value in values {
      this.push_back( value );
    }
  }

  /*
  Empty queue.
  */
  proc clear( ){
    this.size = 0;
  }

  /*
  Compact underlying array to minimal size.
  .. note: This leaves extra no extra capacity for new values.
    Will cause a reallocation after a push-back.
  */
  proc compact(){
    this.listDomain = {1..#this.size};
  }

  /*
  Yield all values from array.
  :ytype: eltType
  */
  iter these(){
    for i in 1..#size {
      yield this.listArray[i];
    }
  }

  proc writeThis( f ){
    f <~> this.listArray[this.listDomain.low..#this.size];
  }
}

/*
Queue for effecient shared-memory parallel work queueing and distributing using
a chuncked loop wait method.
*/
class ParallelWorkQueueChunkedLoopWait {
  type eltType;
  type lockType;

  const maxTasks : uint;
  const startTaskID : uint = 0;
  const taskDomain : domain(1,uint);

  //NOTE: Observed by Louis Jenkins, there is a possibility of False Sharing.
  // One way of possibly addressing this is implemented in ParallelWorkQueueChunkedLoopWaitPadding
  var locks : [taskDomain] unmanaged lockType;
  var queues : [taskDomain] unmanaged Vector(eltType);

  var addCounter : atomic uint;

  var terminated : atomic bool;
  const terminatedRetries : uint;

  proc init( type eltType, type lockType = unmanaged SyncLock, retries : uint = 0, maxTasks : uint = here.maxTaskPar ){
    this.eltType = eltType;
    this.lockType = lockType;

    this.maxTasks = maxTasks;
    assert( maxTasks >= 1 );
    this.taskDomain = {this.startTaskID..#maxTasks};

    this.terminatedRetries = retries;

    this.complete();

    for taskID in this.taskDomain {
      this.locks[taskID] = new unmanaged lockType();
      this.queues[taskID] = new unmanaged Vector(this.eltType);
    }

    this.addCounter.write(startTaskID);

    this.terminated.write(false);
  }

  proc deinit(){
    for taskID in this.taskDomain {
      delete this.locks[taskID];
      delete this.queues[taskID];
    }
  }

  /*
  :returns: Size of the queue in its entirety.
  :rtype: int
  */
  proc size : int {
    // lock all queues then sum. Should provide a consistant view of queue?
    for taskID in this.taskDomain {
      this.locks[taskID].lock();
    }
    var size : int = 0;
    for taskID in this.taskDomain {
      size += this.queues[taskID].size;
    }
    for taskID in this.taskDomain {
      this.locks[taskID].unlock();
    }
    return size;
  }

  /*
  Add value into the queue.
  .. note: This distributes work in a round-robbin fashion.
  :arg value: Value to be enqued.
  :type value: eltType.
  */
  proc add( value : eltType ){
    // Get a taskID to give work to in a round-robbin manner
    // No need to mod counter, since we can safely do the modular arithmatic
    // on our end.
    var taskID = this.addCounter.fetchAdd(1) % this.maxTasks;
    this.locks[taskID].lock();
    this.queues[taskID].push_back( value );
    this.locks[taskID].unlock();
  }

  /*
  Yield work in parallel.
  In this method, each task has a queue that it swaps out with an empty queue
  and yields values from that.
  :ytype: eltType
  */
  iter these( param tag : iterKind ) : eltType
  where tag == iterKind.standalone
  {
    if warnParallelWorkQueueTheseMethod then compilerWarning( "theseChunkedLoopWait standalone");

    // reset termination
    this.terminated.write(false);

    coforall taskID in this.taskDomain {
      var continueLooping = true;
      var countdown = this.terminatedRetries;
      var unlockedQueue = new unmanaged Vector(eltType);

      // main yield loop.
      while continueLooping {
        var terminated = this.terminated.read();
        continueLooping = !terminated || countdown > 0;
        if terminated then countdown -= 1;

        // get task's queue with quick swap of empty local queue
        this.locks[taskID].lock();
        unlockedQueue <=> this.queues[taskID];
        this.locks[taskID].unlock();

        // yield all in local queue
        if unlockedQueue.size > 0 {
          //unlockedQueue.compact();
          //forall work in unlockedQueue.listArray._value.these( tasksPerLocale = maxTasks ) {
          for work in unlockedQueue {
            yield work;
          }
          unlockedQueue.clear();
        } else {
          chpl_task_yield();
        }
      }

      delete unlockedQueue;
    }

    // clear terminated flag
    this.terminated.write(false);
  }

  // dummy iterator
  pragma "no doc"
  iter these( ) : eltType
  { }

  /*
  Signal to the iterator loop that no more work is incomming and should be terminated
  when the queue(s) are exhaused.
  */
  proc terminate(){
    this.terminated.write(true);
  }

  proc writeThis( f ){
    // lock all queues then print. Should provide a consistant view of queue?
    for taskID in this.taskDomain {
      this.locks[taskID].lock();
    }
    var notfirst = false;
    for taskID in this.taskDomain {
      if notfirst then f <~> "\n";
      else notfirst = true;
      f <~> taskID <~> ": " <~> this.queues[taskID];
    }
    for taskID in this.taskDomain {
      this.locks[taskID].unlock();
    }
  }
}

/*
Queue for effecient shared-memory parallel work queueing and distributing using
a chuncked loop wait method. In this implementation, the locks and queues are
unified into one array that uses padding as an approach to reduce the possiblity
of false-sharing.
*/
class ParallelWorkQueueChunkedLoopWaitPadding {
  type eltType;
  type lockType;

  // tuple that will pad the record through the cache line
  type paddingType = ParallelWorkQueueChunkedLoopWaitPaddingBytes*uint(8);

  record arrayType {
    type arrayTypeEltType;
    type arrayTypeLockType;
    type arrayTypePaddingType;

    var queue : unmanaged Vector(arrayTypeEltType);
    var lock : unmanaged arrayTypeLockType;
    var padding : arrayTypePaddingType;
  }

  const maxTasks : uint;
  const startTaskID : uint = 0;
  const taskDomain : domain(1,uint);

  var queuesLocksPaddingArray : [taskDomain] arrayType(eltType, lockType, paddingType);

  var addCounter : atomic uint;

  var terminated : atomic bool;
  const terminatedRetries : uint;

  proc init( type eltType, type lockType = unmanaged SyncLock, retries : uint = 0, maxTasks : uint = here.maxTaskPar ){
    this.eltType = eltType;
    this.lockType = lockType;

    this.maxTasks = maxTasks;
    assert( maxTasks >= 1 );
    this.taskDomain = {this.startTaskID..#maxTasks};

    this.terminatedRetries = retries;

    this.complete();

    for taskID in this.taskDomain {
      this.queuesLocksPaddingArray[taskID].lock = new unmanaged lockType();
      this.queuesLocksPaddingArray[taskID].queue = new unmanaged Vector(this.eltType);
    }

    this.addCounter.write(startTaskID);

    this.terminated.write(false);
  }

  proc deinit(){
    for taskID in this.taskDomain {
      delete this.queuesLocksPaddingArray[taskID].lock;
      delete this.queuesLocksPaddingArray[taskID].queue;
    }
  }

  /*
  :returns: Size of the queue in its entirety.
  :rtype: int
  */
  proc size : int {
    // lock all queues then sum. Should provide a consistant view of queue?
    for taskID in this.taskDomain {
      this.queuesLocksPaddingArray[taskID].lock.lock();
    }
    var size : int = 0;
    for taskID in this.taskDomain {
      size += this.queuesLocksPaddingArray[taskID].queue.size;
    }
    for taskID in this.taskDomain {
      this.queuesLocksPaddingArray[taskID].lock.unlock();
    }
    return size;
  }

  /*
  Add value into the queue.
  .. note: This distributes work in a round-robbin fashion.
  :arg value: Value to be enqued.
  :type value: eltType.
  */
  proc add( value : eltType ){
    // Get a taskID to give work to in a round-robbin manner
    // No need to mod counter, since we can safely do the modular arithmatic
    // on our end.
    var taskID = this.addCounter.fetchAdd(1) % this.maxTasks;
    this.queuesLocksPaddingArray[taskID].lock.lock();
    this.queuesLocksPaddingArray[taskID].queue.push_back( value );
    this.queuesLocksPaddingArray[taskID].lock.unlock();
  }

  /*
  Yield work in parallel.
  In this method, each task has a queue that it swaps out with an empty queue
  and yields values from that.

  :ytype: eltType
  */
  iter these( param tag : iterKind ) : eltType
  where tag == iterKind.standalone
  {
    if warnParallelWorkQueueTheseMethod then compilerWarning( "theseChunkedLoopWait standalone");

    // reset termination
    this.terminated.write(false);

    coforall taskID in this.taskDomain {
      var continueLooping = true;
      var countdown = this.terminatedRetries;
      var unlockedQueue = new unmanaged Vector(eltType);

      // main yield loop.
      while continueLooping {
        var terminated = this.terminated.read();
        continueLooping = !terminated || countdown > 0;
        if terminated then countdown -= 1;

        // get task's queue with quick swap of empty local queue
        this.queuesLocksPaddingArray[taskID].lock.lock();
        unlockedQueue <=> this.queuesLocksPaddingArray[taskID].queue;
        this.queuesLocksPaddingArray[taskID].lock.unlock();

        // yield all in local queue
        if unlockedQueue.size > 0 {
          //unlockedQueue.compact();
          //forall work in unlockedQueue.listArray._value.these( tasksPerLocale = maxTasks ) {
          for work in unlockedQueue {
            yield work;
          }
          unlockedQueue.clear();
        } else {
          chpl_task_yield();
        }
      }

      delete unlockedQueue;
    }

    // clear terminated flag
    this.terminated.write(false);
  }

  // dummy iterator
  pragma "no doc"
  iter these( ) : eltType
  { }

  /*
  Signal to the iterator loop that no more work is incomming and should be terminated
  when the queue(s) are exhaused.
  */
  proc terminate(){
    this.terminated.write(true);
  }

  proc writeThis( f ){
    // lock all queues then print. Should provide a consistant view of queue?
    for taskID in this.taskDomain {
      this.queuesLocksPaddingArray[taskID].lock.lock();
    }
    var notfirst = false;
    for taskID in this.taskDomain {
      if notfirst then f <~> "\n";
      else notfirst = true;
      f <~> taskID <~> ": " <~> this.queuesLocksPaddingArray[taskID].queue;
    }
    for taskID in this.taskDomain {
      this.queuesLocksPaddingArray[taskID].lock.unlock();
    }
  }
}


/*
Queue for effecient shared-memory parallel work queueing and distributing using
a sync wait method.
*/
class ParallelWorkQueueSyncWait {
  type eltType;
  type lockType;

  const maxTasks : uint;
  const startTaskID : uint = 0;
  const taskDomain : domain(1,uint);

  var lock : unmanaged lockType;
  var queue : unmanaged list(eltType);

  var terminated : atomic bool;
  const terminatedRetries : uint;
  const reloopCycles : uint;

  proc init( type eltType, type lockType = unmanaged SyncLock, retries : uint = 0, reloopCycles : uint = 3, maxTasks : uint = here.maxTaskPar ){
    this.eltType = eltType;
    this.lockType = lockType;

    this.maxTasks = maxTasks;
    assert( maxTasks >= 1 );
    this.taskDomain = {this.startTaskID..#maxTasks};

    this.lock = new unmanaged lockType();

    this.terminatedRetries = retries;
    this.reloopCycles = reloopCycles;

    this.complete();

    this.terminated.write(false);
  }

  proc deinit(){
    delete this.lock;
  }

  /*
  .. note: this is not globally serialized.
    It is possible that the size returned is different than what actually exists
    in the queues.
  :returns: Size of the queue in its entirety.
  :rtype: int
  */
  proc size : int {
    this.lock.lock();
    const retval = this.queue.size;
    this.lock.unlock();
    return retval;
  }

  /*
  Add value into the queue.
  .. note: This distributes work in a round-robbin fashion.
  :arg value: Value to be enqued.
  :type value: eltType.
  */
  proc add( value : eltType ){
    this.lock.lock();
    this.queue.push_back( value );
    this.lock.unlock();
  }

  /*
  Yield work in parallel.
  In this method, each tasks has a sync variable containing that it waits on.
  A manager task doles out work from a single queue round-robbin to those variables.
  When it's task-local queue is exhausted, it swaps the global queue with the empty one.
  .. note: This is not supported in this queue implementation. TODO should be made it's own
    queue object.
  :ytype: eltType.
  */
  iter these(param tag : iterKind ) : eltType
  where tag == iterKind.standalone
  {
    if warnParallelWorkQueueTheseMethod then compilerWarning("theseSyncWait theseChunkedLoopWait");
    // reset termination
    this.terminated.write(false);
    enum WorkSignal { Work, Die };
    var signals : [startTaskID..#maxTasks] WorkSignal;
    var work : [startTaskID..#maxTasks] sync eltType;

    // setup work signals (only changed when killed)
    for signal in signals {
      signal = WorkSignal.Work;
    }

    // begin main tasks
    cobegin {
      // Manager Task
      {
        var continueLooping = true;
        var countdown = this.terminatedRetries;
        const reloops = reloopCycles*maxTasks;
        var unlockedQueue = new list(eltType);
        var workertaskID = this.startTaskID;

        // yield loop.
        while continueLooping {
          var terminated = this.terminated.read();
          continueLooping = !terminated || countdown > 0;
          if terminated then countdown -= 1;

          // exchange empty unlocked queue for the active locked queue
          if unlockedQueue.size == 0 {
            this.lock.lock();
            unlockedQueue <=> this.queue;
            this.lock.unlock();
          }

          // yield work (if any) from unlockedQueue
          {
            var giveWork = true;
            var loops = reloops;

            while giveWork {
              // if no work, or max loops reached
              if unlockedQueue.size == 0 || loops == 0 {
               giveWork = false;
              } else {
                // if cannot dole work to this task, decrement loops,
                if work[workertaskID].isFull {
                  loops -= 1;
                } else {
                  // Get work from queue
                  var value = unlockedQueue.pop_front();
                  // give work (non-blocking by arrangement);
                  work[workertaskID].writeEF( value );
                }
                // go to next task
                workertaskID = (workertaskID + 1) % maxTasks;
              }
            }
          }

          chpl_task_yield();
        } // while continueLooping

        // kill tasks loop
        const dummyValue : eltType;
        for taskID in startTaskID..#maxTasks {
          // set kill signal
          signals[taskID] = ( WorkSignal.Die );
          // give dummy value to wake task, which immediately check signal and dies.
          work[taskID].writeEF(dummyValue);
        }
      } // begin manager task

      // Worker Tasks
      {
        coforall taskID in startTaskID..#maxTasks {
          var continueLooping = true;
          while continueLooping {
            // wait for work (blocking)
            var value = work[taskID].readFE();
            // check signal (non-blocking)
            var signal = signals[taskID];
            select signal {
              when WorkSignal.Work do {
                yield value;
              }
              when WorkSignal.Die do {
                continueLooping = false;
              }
              otherwise do {
                halt("Task ", taskID, " got bad signal: ", signal );
              }
            }
          } // while continueLooping
        } // coforall
      } // begin worker tasks

    }// cobegin

    // clear terminated flag
    this.terminated.write(false);
  }

  // dummy iterator
  pragma "no doc"
  iter these( ) : eltType
  {
    // compilerError("ParallelWorkQueue.theseSyncWait serial being called");
  }

  /*
  Signal to the iterator loop that no more work is incomming and should be terminated
  when the queue(s) are exhaused.
  */
  proc terminate(){
    this.terminated.write(true);
  }

  proc writeThis( f ){
    this.lock.lock();
    f <~> this.queue;
    this.lock.unlock();
  }
}

/*
Queue for effecient shared-memory parallel work queueing and distributing using
a loop wait method.
*/
class ParallelWorkQueueLoopWait {
  type eltType;
  type lockType;

  const maxTasks : uint;
  const startTaskID : uint = 0;
  const taskDomain : domain(1,uint);

  var lock : unmanaged lockType;
  var queue : unmanaged list(eltType);

  var terminated : atomic bool;
  const terminatedRetries : uint;

  proc init( type eltType, type lockType = unmanaged SyncLock, retries : uint = 0, maxTasks : uint = here.maxTaskPar ){
    this.eltType = eltType;
    this.lockType = lockType;

    this.maxTasks = maxTasks;
    assert( maxTasks >= 1 );
    this.taskDomain = {this.startTaskID..#maxTasks};

    this.lock = new unmanaged lockType();

    this.terminatedRetries = retries;

    this.complete();

    this.terminated.write(false);
  }

  proc deinit(){
    delete this.lock;
  }

  /*
  .. note: this is not globally serialized.
    It is possible that the size returned is different than what actually exists
    in the queues.
  :returns: Size of the queue in its entirety.
  :rtype: int
  */
  proc size : int {
    this.lock.lock();
    const retval = this.queue.size;
    this.lock.unlock();
    return retval;
  }

  /*
  Add value into the queue.
  .. note: This distributes work in a round-robbin fashion.
  :arg value: Value to be enqued.
  :type value: eltType.
  */
  proc add( value : eltType ){
    this.lock.lock();
    this.queue.push_back( value );
    this.lock.unlock();
  }

  /*
  Yield work in parallel.
  In this method, all tasks share a queue, and a task will get one value from the queue.
  :ytype: eltType.
  */
  iter these( param tag : iterKind ) : eltType
  where tag == iterKind.standalone
  {
    if warnParallelWorkQueueTheseMethod then compilerWarning( "theseLoopWait standalone");
    // reset termination
    this.terminated.write(false);

    coforall taskID in taskDomain {
      var continueLooping = true;
      var countdown = this.terminatedRetries;

      // main yield loop.
      while continueLooping {
        var terminated = this.terminated.read();
        continueLooping = !terminated || countdown > 0;
        if terminated then countdown -= 1;

        this.lock.lock();
        if this.queue.size > 0 {
          var value = this.queue.pop_front();
          this.lock.unlock();
          yield value;
        } else {
          this.lock.unlock();
          chpl_task_yield();
        }
      } // while
    } // coforall taskID

    // clear terminated flag
    this.terminated.write(false);
  }

  // dummy iterator
  pragma "no doc"
  iter these( ) : eltType
  {
    //compilerError("ParallelWorkQueue.theseLoopWait serial being called");
  }

  /*
  Signal to the iterator loop that no more work is incomming and should be terminated
  when the queue(s) are exhaused.
  */
  proc terminate(){
    this.terminated.write(true);
  }

  proc writeThis( f ){
    this.lock.lock();
    f <~> this.queue;
    this.lock.unlock();
  }
}

/*
Queue for effecient distributed-memory parallel work queueing and distributing.
*/
pragma "always RVF"
record DistributedWorkQueue {
  type eltType;
  type lockType;

  var localesDomain : domain(1) = {1..0};
  var locales : [localesDomain] locale;

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

    this.localesDomain = {0..#targetLocales.domain.size};

    this.complete();
    this.localInstance = new unmanaged LocalDistributedWorkQueue(eltType, lockType, targetLocales);
    this.pid = this.localInstance.pid;
  }

  proc deinit(){
    delete this.localInstance;
  }

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

/*
Acutal/Local implementation of DistributedWorkQueue.
*/
class LocalDistributedWorkQueue {
  type eltType;
  type lockType;

  const localeDomain : domain(1);
  const localeArray : [localeDomain] locale;

  var lock : unmanaged lockType;
  var queue : unmanaged Vector(eltType);
  var terminated : atomic bool;
  const terminatedRetries : int;

  var pid = -1;

  proc init( type eltType, type lockType, localeArray : [?localeDomain] locale, retries : int = 5 ){
    this.eltType = eltType;
    this.lockType = lockType;
    this.localeDomain = {0..#localeDomain.size};
    this.localeArray = reshape( localeArray, {0..#localeDomain.size} );
    this.lock = new unmanaged lockType();
    this.queue = new unmanaged Vector(eltType);
    this.terminatedRetries = retries;

    this.complete();

    terminated.write(false);
    this.pid = _newPrivatizedClass(this);
  }

  proc init( that : unmanaged LocalDistributedWorkQueue(?eltType, ?lockType), pid : int) {
    this.eltType = eltType;
    this.lockType = lockType;
    this.localeDomain = that.localeDomain;
    this.localeArray = that.localeArray;
    this.lock = new unmanaged lockType();
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

  /*
  :returns: PID of local queue.
  */
  pragma "no doc"
  proc dsiGetPrivatizeData() {
    return pid;
  }

  /*
  :arg pid: PID of local queue that should be returned
  :returns: local queue given by PID
  :rtype: unmanaged LocalDistributedWorkQueue
  */
  pragma "no doc"
  proc dsiPrivatize(pid) {
    return new unmanaged LocalDistributedWorkQueue(unmanaged this, pid);
  }

  /*
  Get self local copy (?)
  */
  pragma "no doc"
  inline proc getPrivatizedThis {
    return chpl_getPrivatizedCopy(this.type, this.pid);
  }

  /*
  Add work onto owner's queue.
  :arg value: value to be enqueued.
  :type value: eltType.
  :arg owner: locale who should execute that work.
  :type owner: Locale.
  */
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

  /*
  Yield work in parallel over all locales.
  In this method, each locale has a queue. That queue is swapped with an empty queue
  for other tasks to add to, and yields values in parallel from the full queue.
  Does this in a loop wait fashion.
  */
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

  /*
  Signal to the iterator loop that no more work is incomming and should be terminated
  when the queue(s) are exhaused.
  */
  proc terminate(){
    coforall onLocale in this.localeArray {
      on onLocale {
        var instance = getPrivatizedThis;
        instance.terminated.write(true);
      }
    }
  }
}

/*
Class for storing and utilizing a permutation map.
*/
class PermutationMap {
  type idxType;
  param rank = 2;
  var rowDom : domain(1);
  var columnDom : domain(1);
  var rowMap : [rowDom] idxType;
  var columnMap : [columnDom] idxType;

  /*
  Construct a PermuataionMap given input arrays representing the row map
  and the column map.
  :arg rowMap: array representing the mapping from rows (represented by the domain of this array)
  to new rows in the permutation map (represented by the values of this array)
  (e.g. rowMap[i] = j means row i is mapped to row j)
  :type rowMap: [] ?idxType.
  :arg rowMap: array representing the mapping from columns (represented by the domain of this array)
  to new columns in the permutation map (represented by the values of this array)
  (e.g. columnMap[i] = j means column i is mapped to column j)
  :type columnMap: [] idxType.
  */
  proc init( rowMap : [] ?idxType, columnMap : [] idxType ){
    this.idxType = idxType;
    this.rowDom = rowMap.domain;
    this.columnDom = columnMap.domain;
    this.rowMap = rowMap;
    this.columnMap = columnMap;
  }

  /*
  Permute an index-tuple accoriding to the mapping
  :arg idx: index to be permuted.
  :type idx: rank*idxType.
  :returns: Permuted index tuple.
  rtype: rank*idxType.
  */
  inline proc map( idx : rank*idxType ) : rank*idxType {
    return (rowMap[idx[1]], columnMap[idx[2]]);
  }

  /*
  Take a permuted index-tuple and get the original input index-tuple.
  :arg idx: index to be 'unpermuted'
  :type idx: rank*idxType.
  :returns: 'unpermuted' index tuple.
  rtype: rank*idxType.
  */
  inline proc inverseMap( idx : rank*idxType ) : rank*idxType {
    return ( linearSearch(rowMap, idx[1]), linearSearch(rowMap, idx[1]) );
  }

  /*
  Same as map.
  */
  inline proc this( idx : rank*idxType ) : rank*idxType {
    return map( idx );
  }

  /*
  Create a new PermutationMap object that maps the inverse of
  this permutation map.
  :rtype: owned PermutationMap(idxType)
  */
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

  /*
  Given a domain iterate over the mapped indexes/tuples.
  :arg onDomain: input domain to be iterated over.
  :type onDomain: domain(rank), usually sparse, but could be dense.
  :yields: permuted indices.
  :ytype: rank*idxType
  */
  iter these( onDomain : domain ) : rank*idxType
  where onDomain.rank == 2
  {
    for idx in onDomain do yield this.map( idx );
  }

  /*
  Given a domain iterate over the inversely mapped indexes/tuples.
  :arg onDomain: permuted domain to be iterated over.
  :type onDomain: domain(rank), usually sparse, but could be dense.
  :yields: 'unpermuted' indices.
  :ytype: rank*idxType
  */
  iter inverseThese( onDomain : domain ) : rank*idxType
  where onDomain.rank == 2
  {
    for idx in onDomain do yield inverseMap( idx );
  }

  /*
  Given a domain iterate over the mapped indexes/tuples.
  :arg onDomain: input domain to be iterated over.
  :type onDomain: domain(rank), usually sparse, but could be dense.
  :yields: permuted indices.
  :ytype: rank*idxType
  */
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

  /*
  Given a sparse domain, return a new sparse domain of those indices permuted.
  :arg D: Input domain whose indices will be permuted and form the output domain
  :type D: sparse domain(rank)
  :returns: sparse domain of permuted indices.
  :rtype: sparse domain(rank)
  */
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

  /*
  Given an array of indices/tuples, return a new array of indices/tuples
  of those indices permuted.
  :arg array: Input array whose indices will be the output domain
  :type array: [?arrayDomain] rank*idxType
  :returns: array of permuted indices
  :rtype: [arrayDomain] rank*idxType
  */
  proc permuateIndexList( array : [?arrayDomain] rank*idxType ) : [arrayDomain] rank*idxType {
    var retArray : [array.domain] array.eltType;
    forall i in retArray.domain {
      retArray[i] = this( array[i] );
    }
    return retArray;
  }
}

/*
Encapsulates all the objects that would be returned by any toposort implementation
In particular, this stores the resulting PermutationMap object, and the various
timers that are standard to testing ("whole", "initialization", and "toposort")
along with any new timers that the implementation wishes to add.
*/
class TopoSortResult {
  type idxType;
  // Solved PermutationMap
  var permutationMap : shared PermutationMap(idxType);
  // Domain of timer names.
  var timerDom : domain(string);
  // Array of timers.
  var timers : [timerDom] Timer;

  proc init(type idxType){
    this.idxType = idxType;
    this.permutationMap = nil;
    this.timerDom = {"whole","initialization","toposort"};
  }
}

/*
Given a input domain and a random number seed, produce a random PermutationMap instance.
:arg D: Domain that permutation map will work in.
:type D: domain(2) (could be sparse or dense).
:arg seed: Seed for random number generator.
:returns: PermutationMap with a permutation generated randomly from seed over the domain D.
:rtype: shared PermtuationMap(D.idxType)
*/
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

/*
Create a array of indices representing a subdomain of D, which is
Upper Triangular in form (ie for (i,j) in array, i <= j).
:arg density: Approximate Real NumberNonZero/(N*N) density of resulting index set.
Must be no more than the maximum density of an UT index set ( N*(N+1)/2)
and no less than the miniumn density of an UT index set (N).
:type density: real
:arg seed: Random number seed used to generate the indexes.
:type seed: int
:arg fillModeDensity: density at which to use the more memory hungry, but potentially
faster index selection method.
:type fillModeDensity: real
:returns: Array of indices representing a sparse subdomain of of D with given density.
:rtype: [] D.rank*D.idxType
*/
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
      var random = new owned RandomStream(D.idxType, seed);

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
        var localRandom = new owned RandomStream(D.idxType, (seed ^ (row << 1) | 1) );
        // our index into sDRandomDom
        var i = startOffset[row];
        // set of indices we already have
        var bagOfIndices : domain(D.idxType);
        while rowCount[row] > 0 {
          // random column in range row+1 .. high
          var column = (abs( localRandom.getNext() ) % (high-row)) + row+1;
          // if not already in our index set, add it
          if !bagOfIndices.member( column ) {
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

/*
Given some iterable collection of indices, return true if the index set conforms
to an Upper Triangular format, false otherwise.
:arg collection: Some iterable collection of 2*T values (usually T==int)
:type collection: ?collectionType (generic). Must support `these` which yields `2*T` type values
:rtype: bool
*/
proc checkIsUperTriangularIndexCollection( collection : ?collectionType ) : bool
{
  var isUT = true;
  for (i,j) in collection {
    isUT = isUT && (i <= j);
    if !isUT then break;
  }
  return isUT;
}

/*
Method for writing a sparse matrix/array to stdout.
M: sparse matrix/array
printIRV: when writing a 'zero' value, write the IRV value
(default=false; writes a formatted space)
separateElements: insert a space between two adjacent values. (default true; writes a single space)
*/
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

/*
Serial implementation of Toposort.
:arg D: Sparse domain to be toposorted.
:type D: sparse domain(2)
:returns: Filled TopoSortResult instance.
:rtype: shared TopoSortResult(D.idxType)
*/
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
  var workQueue : list(D.idxType);

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  // iterate in dense dimension (column) and then through the sparse dimensions
  // (rows) and sum into global atomic arrays
  for column in columns {
    if enableRuntimeDebugging && debugTopo then writeln( "accumulating in column ", column );
    if useDimIterRow {
     if warnDimIterMethod then compilerWarning("toposortSerial.init iterating over rows in init with dimIter");
      for row in D.dimIter(1,column) {
        rowCount[row] += 1;
        rowSum[row] += column ;
      }
    } else {
      if warnDimIterMethod then compilerWarning("toposortSerial.init iterating over rows in init with dim");
      for row in rows {
        if D.member((row,column)) {
          rowCount[row] += 1;
          rowSum[row] += column;
        }
      }
    }
  }

  // Queue ready rows
  for row in rows {
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

/*
Parallel implementation of toposort.
:arg D: Sparse domain to be toposorted.
:type D: sparse domain(2)
:arg workQueue: An instance of the work queue used for queuing and distributing work
among tasks.
:type workQueue: ?queueType (generic). Must support: add(value:int), these(tasksPerLocale:int, iterKind)
:arg numTasks: the maximum number of tasks that the benchmark is allowed to use in
parallel sections (default=here.maxTaskPar).
:type numTasks: int.
:returns: Filled TopoSortResult instance.
:rtype: shared TopoSortResult(D.idxType)
*/
proc toposortParallel( D : domain, workQueue : ?queueType, numTasks : int = here.maxTaskPar ) : shared TopoSortResult(D.idxType)
where D.rank == 2
{
  if numTasks < 1 then halt("Must run with numTaks >= 1");

  var result = new shared TopoSortResult(D.idxType);
  result.timers["whole"].start();

  const rows : domain(1) = {D.dim(1)};
  const columns : domain(1) = {D.dim(2)};
  const numDiagonals = min( rows.size, columns.size );

  var rowMap : [rows] D.idxType = [i in rows] -1;
  var columnMap : [columns] D.idxType = [i in columns] -1;

  var rowSum : [rows] atomic int;
  var rowCount : [rows] atomic int;

  // initialize rowCount and rowSum and put work in queue
  result.timers["initialization"].start();
  // iterate in dense dimension (column) and then through the sparse dimensions
  // (rows) and sum into global atomic arrays
  forall column in columns._value.these(tasksPerLocale=numTasks) {
    if enableRuntimeDebugging && debugTopo then writeln( "accumulating in column ", column );
    if useDimIterRow {
     if warnDimIterMethod then compilerWarning("toposortParallel.init iterating over rows in init with dimIter");
      for row in D.dimIter(1,column) {
        rowCount[row].add(1);
        rowSum[row].add(column);
      }
    } else {
      if warnDimIterMethod then compilerWarning("toposortParallel.init iterating over rows in init with dim");
      for row in rows {
        if D.member((row,column)) {
          rowCount[row].add(1);
          rowSum[row].add(column);
        }
      }
    }
  }

  // Queue ready rows
  forall row in rows._value.these(tasksPerLocale=numTasks) {
    if rowCount[row].read() == 1 {
      workQueue.add( row );
    }
  }

  result.timers["initialization"].stop();

  if enableRuntimeDebugging && debugTopo {
    writeln( "initial workQueue ", workQueue );
    writeln( "initial rowSum    ", rowSum );
    writeln( "initial rowCount  ", rowCount );
  }

  if workQueue.size < 1 then halt("Work queue is empty!");

  // insert position along diagonal from (N,N)
  var diagonalPosition : atomic int;
  diagonalPosition.write(0);

  result.timers["toposort"].start();

  // For each queued row (and rows that will be queued)...
  forall swapRow in workQueue {
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

/*
Distributed implementation of toposort.
This is a helper method for the real toposortDistributed method that accepts
a list of each locale's maxTaskPerLocal parameter, which sets that array to
that locales default (here.maxTaskPar).
:arg D: sparse domain to be toposorted.
:type D: sparse domain(2)
:returns: Filled TopoSortResult instance.
:rtype: shared TopoSortResult(D.idxType)
*/
proc toposortDistributed( D : domain ) : shared TopoSortResult(D.idxType)
where D.rank == 2
{
  var maxTasksPerLocale : [0..#Locales.size] int;
  for onLocale in Locales {
    maxTasksPerLocale[onLocale.id] = onLocale.maxTaskPar;
  }
  return toposortDistributed( D, maxTasksPerLocale );
}

/*
Distributed implementation of toposort.
:arg D: sparse domain to be toposorted.
:type D: sparse domain(2)
:arg maxTaskPerLocael: maximum number of tasks for each locale that the
benchmark is allowed to use in parallel sections.
:type maxTasksPerLocale: [0..#Locales.size] int
:returns: Filled TopoSortResult instance.
:rtype: shared TopoSortResult(D.idxType)
*/
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
  var workQueue = new DistributedWorkQueue(D.idxType, Locales);

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

/*
Enumeration representing each implementation.
This includes which queue to use.
*/
enum ToposortImplementation {
  // Use serial implementation
  Serial,
  // Use parallel implementation with ParallelWorkQueueChunkedLoopWait as queue
  ParallelChunkedLoopWait,
  // Use parallel implementation with ParallelWorkQueueChunkedLoopWaitPadding as queue
  ParallelChunkedLoopWaitPadding,
  // Use parallel implementation with ParallelWorkQueueSyncWait as queue
  ParallelSyncWait,
  // Use parallel implementation with ParallelWorkQueueLoopWait as queue
  ParallelLoopWait,
  // Use distributed implementation
  Distributed
};
/*
Flag for user to specify which toposort implementation they would like.
*/
config const implementation : ToposortImplementation = ToposortImplementation.ParallelChunkedLoopWait;

config const printImplementations : bool = false;

proc main(){
  if printImplementations then {
    writeln("Available implementation options:");
    for i in ToposortImplementation {
      writeln( i );
    }
    return;
  }

  if density < minDensity then halt("Specified density (%n) is less than min density (%n) for N (%n)".format( density, minDensity, N));
  if density > maxDensity then halt("Specified density (%n) is greater than max density (%n) for N (%n)".format( density, maxDensity, N));

  if defaultFillModeDensity > 1 || defaultFillModeDensity < 0 then halt("defaultFillModeDensity must be in [0.0, 1.0] (is %n)".format(defaultFillModeDensity));

  if !silentMode then writeln( "Number of tasks: %n\nN: %n\nSpecified density: %dr%%".format(numTasks, N, density * 100.0 ) );

  // create upper triangular matrix
  if !silentMode then writeln("Creating sparse upper triangluar domain");
  const D : domain(2) = {1..#N,1..#N};

  const sparseUpperTriangularIndexList = createSparseUpperTriangluarIndexList( D, density, seed, defaultFillModeDensity );

  if !silentMode then writeln( "Actual Density: %dr%%\nTotal Number NonZeros: %n".format((sparseUpperTriangularIndexList.size / (1.0*N*N))*100, sparseUpperTriangularIndexList.size) );

  var permutationMap = createRandomPermutationMap( D, seed );
  if printPermutations then writeln("Permutation Map:\n", permutationMap);

  if !silentMode then writeln("Permuting upper triangluar domain");
  var permutedSparseUpperTriangularIndexList = permutationMap.permuateIndexList( sparseUpperTriangularIndexList );

  var topoResult : shared TopoSortResult(D.idxType);

  select implementation {
    when ToposortImplementation.Serial {
       if !silentMode then writeln("Converting to CSC domain");

      var dmappedPermutedSparseD : sparse subdomain(D) dmapped CS(compressRows=false);
      dmappedPermutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );

      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Serial implementation.");
      topoResult = toposortSerial( dmappedPermutedSparseD );
    }

    when ToposortImplementation.ParallelChunkedLoopWait {
       if !silentMode then writeln("Converting to CSC domain");

      var dmappedPermutedSparseD : sparse subdomain(D) dmapped CS(compressRows=false);
      dmappedPermutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );

      var workQueue = new unmanaged ParallelWorkQueueChunkedLoopWait( eltType = dmappedPermutedSparseD.idxType, lockType = unmanaged AtomicLock, retries = 0, maxTasks = numTasks : uint );

      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Parallel implementation with Chunked-Loop-Wait work queue.");
      topoResult = toposortParallel( dmappedPermutedSparseD, workQueue, numTasks );

      delete workQueue;
    }

    when ToposortImplementation.ParallelChunkedLoopWaitPadding  {
       if !silentMode then writeln("Converting to CSC domain");

      var dmappedPermutedSparseD : sparse subdomain(D) dmapped CS(compressRows=false);
      dmappedPermutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );

      var workQueue = new unmanaged ParallelWorkQueueChunkedLoopWaitPadding( eltType = dmappedPermutedSparseD.idxType, lockType = unmanaged AtomicLock, retries = 0, maxTasks = numTasks : uint );

      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Parallel implementation with Chunked-Loop-Wait padded work queue.");
      topoResult = toposortParallel( dmappedPermutedSparseD, workQueue, numTasks );

      delete workQueue;
    }

    when ToposortImplementation.ParallelSyncWait {
       if !silentMode then writeln("Converting to CSC domain");

      var dmappedPermutedSparseD : sparse subdomain(D) dmapped CS(compressRows=false);
      dmappedPermutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );

      var workQueue = new unmanaged ParallelWorkQueueSyncWait( eltType = dmappedPermutedSparseD.idxType, lockType = unmanaged AtomicLock, retries = 0, reloopCycles = 3, maxTasks = numTasks : uint );

      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Parallel implementation with Sync-Wait work queue.");
      topoResult = toposortParallel( dmappedPermutedSparseD, workQueue, numTasks );

      delete workQueue;
    }

    when ToposortImplementation.ParallelLoopWait {
       if !silentMode then writeln("Converting to CSC domain");

      var dmappedPermutedSparseD : sparse subdomain(D) dmapped CS(compressRows=false);
      dmappedPermutedSparseD.bulkAdd( permutedSparseUpperTriangularIndexList );

      var workQueue = new unmanaged ParallelWorkQueueLoopWait( eltType = dmappedPermutedSparseD.idxType, lockType = unmanaged AtomicLock, retries = 0, maxTasks = numTasks : uint );

      if !silentMode then writeln("Toposorting permuted upper triangluar domain using Parallel implementation with Loop-Wait work queue.");
      topoResult = toposortParallel( dmappedPermutedSparseD, workQueue, numTasks );

      delete workQueue;
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

  var solvedMap = topoResult.permutationMap;

  if printPerfStats {
    writeln( "Benchmark timers:");
    for timerName in topoResult.timerDom {
      writeln(timerName, ": ", topoResult.timers[timerName].elapsed() );
    }
    writeln( "Rows/second: ", (N/topoResult.timers["whole"].elapsed()) );
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

  var isUpperTriangular = checkIsUperTriangularIndexCollection( solvedPermutedIndexList );
  if !isUpperTriangular then halt("Solved-permuted permuted upper triangluar domain is not upper triangular!");
}
