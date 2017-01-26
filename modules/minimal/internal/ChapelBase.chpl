/*
 * Copyright 2004-2017 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

// ChapelBase.chpl
//

module ChapelBase {

  //
  // internal reference type
  //
  pragma "ref"
  pragma "no default functions"
  pragma "no object"
  class _ref {
    var _val;
  }

  pragma "compiler generated"
  pragma "unalias fn"
  inline proc chpl__unalias(ref x) { }

/////////////////////////////////////////////////////////////////////////////
// integers

  proc _defaultOf(type it) where it == int return 0;
  proc =(ref a, b) { __primitive("=", a, b); }

/////////////////////////////////////////////////////////////////////////////
// string support

  record string {}
  proc string.string(args...) { }

  pragma "donor fn"
  pragma "auto copy fn"
  pragma "no doc"
  proc chpl__autoCopy(s: string) return s;
  pragma "auto destroy fn"
  proc chpl__autoDestroy(s: string) { }

  pragma "data class"
  pragma "no object"
  pragma "no default functions"
  pragma "no wide class"
  pragma "c_ptr class"
  class c_ptr {
    /* The type that this pointer points to */
    type eltType;
    /* Retrieve the i'th element (zero based) from a pointer to an array.
      Does the equivalent of ptr[i] in C.
    */
    inline proc this(i: integral) ref {
      return __primitive("array_get", this, i);
    }
    /* Get element pointed to directly by this pointer. If the pointer
      refers to an array, this will return ptr[0].
    */
    inline proc deref() ref {
      return __primitive("array_get", this, 0);
    }
  }

  inline proc _cast(type t, arg) return __primitive("cast", t, arg);

/////////////////////////////////////////////////////////////////////////////
// iterator support

  pragma "allocator"
  pragma "locale model alloc"
  extern proc chpl_here_alloc(size:int, md:int(16)): c_void_ptr;

  pragma "locale model free"
  extern proc chpl_here_free(ptr:c_void_ptr): void;

  pragma "no doc"
  pragma "allow ref" // needs to to return tuples with refs
  proc iteratorIndex(ic: _iteratorClass) {
    ic.advance();
    return ic.getValue();
  }

  pragma "no doc"
  pragma "expand tuples with values"  // needs to return tuples with refs
  proc iteratorIndex(t: _tuple) {
    pragma "expand tuples with values"
    proc iteratorIndexHelp(t: _tuple, param dim: int) {
      if dim == t.size then
        return _build_tuple_always_allow_ref(iteratorIndex(t(dim)));
      else
        return _build_tuple_always_allow_ref(iteratorIndex(t(dim)),
                                             (...iteratorIndexHelp(t, dim+1)));
    }

    return iteratorIndexHelp(t, 1);
  }

  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(ir: _iteratorRecord) {
    // body modified during call destructors pass
    return ir;
  }
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(ir: _iteratorRecord) {
    // body inserted during call destructors pass
  }
  pragma "unalias fn"
  inline proc chpl__unalias(const ref x:_iteratorRecord) { }

  inline proc _freeIterator(ic: _iteratorClass) {
    chpl_here_free(__primitive("cast_to_void_star", ic));
  }

  inline proc _freeIterator(x: _tuple) {
    _freeIterator(x(1));
    _freeIterator(x(2));
  }

// for parallel iterators, forall loops

  enum iterKind {leader, follower, standalone};
  inline proc ==(param a, param b) param return __primitive("==", a, b);

  // for yielding shadow variables
  pragma "tuple" record _tuple {
    param size : int;
  }

  // tuple value (refs)
  pragma "build tuple"
  inline proc _build_tuple(x...) {
      return x;
  }

  // tuple value allowing refs (ref actuals)
  pragma "allow ref" 
  pragma "build tuple"
  inline proc _build_tuple_always_allow_ref(x...)
    return x;

  // Catch-all initCopy implementation:
  pragma "compiler generated"
  pragma "init copy fn"
  inline proc chpl__initCopy(x) return x;

  pragma "compiler generated"
  pragma "donor fn"
  pragma "auto copy fn"
  inline proc chpl__autoCopy(x) return chpl__initCopy(x);


  pragma "compiler generated"
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(x: ?t) {
    __primitive("call destructor", x);
  }

  inline proc _getIterator(x) {
    return _getIterator(x.these());
  }

  inline proc _getIterator(ic: _iteratorClass)
    return ic;

  proc _getIterator(type t) {
    return _getIterator(t.these());
  }

  inline proc _getIteratorZip(x) {
    return _getIterator(x);
  }

  inline proc _getIteratorZip(type t) {
    return _getIterator(t);
  }

  inline proc _getIteratorZip(x: _tuple) {
    inline proc _getIteratorZipInternal(x: _tuple, param dim: int) {
      if dim == x.size then
        return (_getIterator(x(dim)),);
      else
        return (_getIterator(x(dim)), (..._getIteratorZipInternal(x, dim+1)));
    }
    if x.size == 1 then
      return _getIterator(x(1));
    else
      return _getIteratorZipInternal(x, 1);
  }


/*
  // vass: is this one needed?
  pragma "auto destroy fn"
  inline proc chpl__autoDestroy(ir: _iteratorRecord) {
    // body inserted during call destructors pass
  }
*/

  proc chpl__staticFastFollowCheck(x) param {
/*
    pragma "no copy" const lead = x;
    if isDomain(lead) || isArray(lead) then
      return chpl__staticFastFollowCheck(x, lead);
    else
      return false;
*/
    return true;
  }

  proc chpl__staticFastFollowCheckZip(x: _tuple) param {
/*
    pragma "no copy" const lead = x(1);
    if isDomain(lead) || isArray(lead) then
      return chpl__staticFastFollowCheckZip(x, lead);
    else
*/
      return false;
  }

  proc chpl__dynamicFastFollowCheck(x) {
    return true;
  }

/////////////////////////////////
  pragma "no implicit copy"
  inline proc _toFollower(iterator: _iteratorClass, leaderIndex)
    return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex));

  inline proc _toFollower(ir: _iteratorRecord, leaderIndex) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var follower = _toFollower(ic, leaderIndex);
    _freeIterator(ic);
    return follower;
  }

  inline proc _toFollower(x, leaderIndex) {
    return _toFollower(x.these(), leaderIndex);
  }

  inline proc _toFollowerZip(x, leaderIndex) {
    return _toFollower(x, leaderIndex);
  }

  inline proc _toFollowerZip(x: _tuple, leaderIndex) {
    return _toFollowerZipInternal(x, leaderIndex, 1);
  }

  inline proc _toFollowerZipInternal(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size then
      return (_toFollower(x(dim), leaderIndex),);
    else
      return (_toFollower(x(dim), leaderIndex),
              (..._toFollowerZipInternal(x, leaderIndex, dim+1)));
  }

  // for fast followers et al
  inline proc _cond_test(x) return x != 0;
  inline proc _cond_test(x:bool) return x;
  inline proc _cond_test(param x:bool) param return x;
  inline proc +(param x, param y) param return __primitive("+", x, y);

/////////////////////////////////
  pragma "no implicit copy"
  inline proc _toFastFollower(iterator: _iteratorClass, leaderIndex, fast: bool) {
    return chpl__autoCopy(__primitive("to follower", iterator, leaderIndex, true));
  }

  inline proc _toFastFollower(ir: _iteratorRecord, leaderIndex, fast: bool) {
    pragma "no copy" var ic = _getIterator(ir);
    pragma "no copy" var follower = _toFastFollower(ic, leaderIndex, fast=true);
    _freeIterator(ic);
    return follower;
  }

  inline proc _toFastFollower(x, leaderIndex) {
    if chpl__staticFastFollowCheck(x) then
      return _toFastFollower(_getIterator(x), leaderIndex, fast=true);
    else
      return _toFollower(_getIterator(x), leaderIndex);
  }

  inline proc _toFastFollowerZip(x, leaderIndex) {
    return _toFastFollower(x, leaderIndex);
  }

  inline proc _toFastFollowerZip(x: _tuple, leaderIndex) {
    return _toFastFollowerZip(x, leaderIndex, 1);
  }

  inline proc _toFastFollowerZip(x: _tuple, leaderIndex, param dim: int) {
    if dim == x.size-1 then
      return (_toFastFollowerZip(x(dim), leaderIndex),
              _toFastFollowerZip(x(dim+1), leaderIndex));
    else
      return (_toFastFollowerZip(x(dim), leaderIndex),
              (..._toFastFollowerZip(x, leaderIndex, dim+1)));
  }

/////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// task constructs

  //
  // data structures for naive implementation of end used for
  // sync statements and for joining coforall and cobegin tasks
  //

//  config param useAtomicTaskCnt = true; // was: CHPL_NETWORK_ATOMICS!="none";

  pragma "end count"
  pragma "no default functions"
  class _EndCount {
    type iType;
    type taskType;
    var i: iType,
        taskCnt: taskType,
        taskList: c_void_ptr = _defaultOf(c_void_ptr);
  }

  // This function is called once by the initiating task.  No on
  // statement needed, because the task should be running on the same
  // locale as the sync/coforall/cobegin was initiated on and thus the
  // same locale on which the object is allocated.
  //
  // TODO: 'taskCnt' can sometimes be local even if 'i' has to be remote.
  // It is currently believed that only a remote-begin will want a network
  // atomic 'taskCnt'. There should be a separate argument to control the type
  // of 'taskCnt'.
  pragma "dont disable remote value forwarding"
  inline proc _endCountAlloc(param forceLocalTypes : bool) {
    return new _EndCount(int,int); // avoid atomics
/*
    type taskCntType = if !forceLocalTypes && useAtomicTaskCnt then atomic int
                                           else int;
    if forceLocalTypes {
      return new _EndCount(chpl__processorAtomicType(int), taskCntType);
    } else {
      return new _EndCount(chpl__atomicType(int), taskCntType);
    }
*/
  }

  // Compiler looks for this variable to determine the return type of
  // the "get end count" primitive.
  type _remoteEndCountType = _endCountAlloc(false).type;

  // This function is called once by the initiating task.  As above, no
  // on statement needed.
  pragma "dont disable remote value forwarding"
  inline proc _endCountFree(e: _EndCount) {
    delete e;
  }

  // This function is called by the initiating task once for each new
  // task *before* any of the tasks are started.  As above, no on
  // statement needed.
  pragma "dont disable remote value forwarding"
  pragma "no remote memory fence"
  proc _upEndCount(e: _EndCount, param countRunningTasks=true) {
/*
    if isAtomic(e.taskCnt) {
      e.i.add(1, memory_order_release);
      e.taskCnt.add(1, memory_order_release);
    } else {
      // note that this on statement does not have the usual
      // remote memory fence because of pragma "no remote memory fence"
      // above. So we do an acquire fence before it.
      chpl_rmem_consist_fence(memory_order_release);
      on e {
        e.i.add(1, memory_order_release);
        e.taskCnt += 1;
      }
    }
    if countRunningTasks {
      here.runningTaskCntAdd(1);  // decrement is in _waitEndCount()
    }
*/
  }

  // This function is called once by each newly initiated task.  No on
  // statement is needed because the call to sub() will do a remote
  // fork (on) if needed.
  pragma "dont disable remote value forwarding"
  proc _downEndCount(e: _EndCount) {
/*
    e.i.sub(1, memory_order_release);
*/
  }

  // This function is called once by the initiating task.  As above, no
  // on statement needed.
  pragma "dont disable remote value forwarding"
  proc _waitEndCount(e: _EndCount, param countRunningTasks=true) {
/*
    // See if we can help with any of the started tasks
    chpl_taskListExecute(e.taskList);

    // Remove the task that will just be waiting/yielding in the following
    // waitFor() from the running task count to let others do real work. It is
    // re-added after the waitFor().
    here.runningTaskCntSub(1);

    // Wait for all tasks to finish
    e.i.waitFor(0, memory_order_acquire);

    if countRunningTasks {
      const taskDec = if isAtomic(e.taskCnt) then e.taskCnt.read() else e.taskCnt;
      // taskDec-1 to adjust for the task that was waiting for others to finish
      here.runningTaskCntSub(taskDec-1);  // increment is in _upEndCount()
    } else {
      // re-add the task that was waiting for others to finish
      here.runningTaskCntAdd(1);
    }
*/
  }

  proc _upEndCount(param countRunningTasks=true) {
    var e = __primitive("get end count");
    _upEndCount(e, countRunningTasks);
  }

  proc _downEndCount() {
    var e = __primitive("get end count");
    _downEndCount(e);
  }

  proc _waitEndCount(param countRunningTasks=true) {
    var e = __primitive("get end count");
    _waitEndCount(e, countRunningTasks);
  }

  ///////////

  pragma "dont disable remote value forwarding"
  inline proc _createFieldDefault(type t, init) {
    pragma "no auto destroy" var x: t;
    x = init;
    return x;
  }

  iter chpl_direct_range_iter(low: int(?w), high: int(w), stride: int(w)) {
    yield low;
    yield high;
/*
    const r = low..high by stride;
    for i in r do yield i;
*/
  }

  proc _defaultOf(type it) where it == c_void_ptr return __primitive("cast", it, 0);

/////////////////////////////////////////////////////////////////////////////
// + reductions

  proc chpl__reduceCombine(globalOp, localOp) {
/*
    on globalOp {
      globalOp.lock();
*/
      globalOp.combine(localOp);
/*
      globalOp.unlock();
    }
*/
  }

  inline proc chpl__cleanupLocalOp(globalOp, localOp) {
    // should this be part of chpl__reduceCombine ?
    delete localOp;
  }

  pragma "ReduceScanOp"
  class ReduceScanOp {
    var l: int; // was: atomicbool; // only accessed locally

    proc lock() {
/*
      var lockAttempts = 0,
          maxLockAttempts = (2**10-1);
      while l.testAndSet() {
        lockAttempts += 1;
        if (lockAttempts & maxLockAttempts) == 0 {
          maxLockAttempts >>= 1;
          chpl_task_yield();
        }
      }
*/
    }
    proc unlock() {
/*
      l.clear();
*/
    }
  }
  
  class SumReduceScanOp: ReduceScanOp {
    type eltType;
    var value: eltType; // was: chpl__sumType(eltType);

    // Rely on the default value of the desired type.
    // Todo: is this efficient when that is an array?
    proc identity {
      var x: eltType; // was: chpl__sumType(eltType);
      return x;
    }
    proc accumulate(x) {
      value += x;
    }
    proc accumulateOntoState(ref state, x) {
      state += x;
    }
    proc combine(x) {
      value += x.value;
    }
    proc generate() return value;
    proc clone() return new SumReduceScanOp(eltType=eltType);
  }

  inline proc +=(ref lhs, rhs) {
    __primitive("+=", lhs, rhs);
  }


/////////////////////////////////////////////////////////////////////////////

}
