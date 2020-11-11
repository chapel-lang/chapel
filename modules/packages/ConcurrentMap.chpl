/*
 * Copyright 2004-2019 Cray Inc.
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
prototype module ConcurrentMap {
  use Memory;
  use AtomicObjects;
  use LockFreeStack;
  use LockFreeQueue;
  use EpochManager;
  use Random;
  use VisualDebug;

  param BUCKET_UNLOCKED = 0;
  param BUCKET_LOCKED = 1;
  param BUCKET_DESTROYED = 2;
  config param BUCKET_NUM_ELEMS = 8;
  config const DEFAULT_NUM_BUCKETS = 1024;
  config param MULTIPLIER_NUM_BUCKETS : real = 2;
  config param DEPTH = 2;
  config param EMAX = 4;

  // Note: Once this becomes distributed, we have to make it per-locale
  var seedRNG = new owned RandomStream(uint(64), parSafe=true);

  const E_AVAIL = 1;
  const E_LOCK = 2;
  const P_INNER = 3;
  const P_TERM = 4;
  const P_LOCK = 5;
  const GARBAGE = 6;

  class DeferredNode {
    type eltType;
    var val : eltType;
    var prev : unmanaged DeferredNode(eltType)?;
    var next : unmanaged DeferredNode(eltType)?;

    proc init(type eltType) {
      this.eltType = eltType;
    }

    proc init(val : ?eltType) {
      this.eltType = eltType;
      this.val = val;
    }

    proc deinit() {
      var prevNode = prev;
      var nextNode = next;
      if (prevNode == nil) {
        if (nextNode != nil) then nextNode!.prev = nil;
      } else {
        if (nextNode == nil) then prevNode!.next = nil;
        else {
          prevNode!.next = nextNode;
          nextNode!.prev = prevNode;
        }
      }
    }
  }

  class StackNode {
    type eltType;
    var val : eltType?;
    var next : unmanaged StackNode(eltType?)?;

    proc init(type eltType) {
      this.eltType = eltType;
    }

    proc init(val : ?eltType) {
      this.eltType = eltType;
      this.val = val;
    }
  }

  class Stack {
    type eltType;
    var top : unmanaged StackNode(eltType?)?;
    var count : int;

    proc init(type eltType) {
      this.eltType = eltType;
    }

    proc push(val : eltType?) {
      var node = new unmanaged StackNode(val);
      node.next = top;
      top = node;
      count += 1;
    }

    proc pop() : eltType? {
      if (count > 0) {
        var ret = top.val;
        var next = top.next;
        delete top;
        top = next;
        count -= 1;
        return ret;
      } else {
        var temp : eltType?;
        return temp;
      }
    }

  proc isEmpty() : bool {
    return count == 0;
  }
  }

  // Can be either a singular 'Bucket' or a plural 'Buckets'
  class Base {
    type keyType;
    type valType;
    // If E_AVAIL || E_LOCK, can be cas to `Bucket`
    // if P_INNER, can be cast to `Buckets`
    // if GARBAGE, then reload as its to be destroyed.
    var lock : chpl__processorAtomicType(int);
    // Is always either 'nil' if its the root, or a
    // a 'Buckets', but I cannot make the field of
    // type 'Buckets' as it is not defined yet.
    var parent : unmanaged Base(keyType, valType)?;

    proc init(type keyType, type valType) {
      this.keyType = keyType;
      this.valType = valType;
    }
  }

  // Stores keys and values in the hash table. The lock is used to
  // determine both the 'lock'/'unlock' state of the bucket, and if
  // the bucket is going to be destroyed, meaning that the task should 
  // back out and try again. The bucket gets destroyed when a task
  // attempts to insert an element into an already-full bucket. All
  // tasks _must_ be in the current epoch to even get this far, so
  // this Bucket, even if the lock value is BUCKET_DESTROYED, should
  // not be destroyed until no it is safe to do so.
  class Bucket : Base {
    var count : uint;
    var keys : BUCKET_NUM_ELEMS * keyType;
    var values : BUCKET_NUM_ELEMS * valType;

    proc init(type keyType, type valType) {
      super.init(keyType, valType);
      this.lock.write(E_AVAIL);
    }

    proc init(parent : unmanaged Buckets(?keyType, ?valType) = nil) {
      super.init(keyType, valType);
      this.lock.write(E_AVAIL);
      this.parent = parent;
    }

    proc releaseLock() {
      if (lock.read() == E_LOCK) then lock.write(E_AVAIL);
    }
  }

  class Buckets : Base {
    var seed : uint(64);
    var size : int;
    var bucketsDom = {0..-1};
    var buckets : [bucketsDom] AtomicObject(unmanaged Base(keyType, valType)?, hasABASupport=false, hasGlobalSupport=true);
    // var buckets : [0..(size-1)] AtomicObject(unmanaged Base(keyType?, valType?)?, hasABASupport=false, hasGlobalSupport=true);

    proc init(type keyType, type valType) {
      super.init(keyType, valType);
      this.lock.write(P_INNER);
      this.seed = seedRNG.getNext();
      this.size = DEFAULT_NUM_BUCKETS;
      this.bucketsDom = {0..#DEFAULT_NUM_BUCKETS};
    }

    proc init(parent : unmanaged Buckets(?keyType, ?valType)) {
      super.init(keyType, valType);
      this.seed = seedRNG.getNext();
      this.lock.write(P_INNER);
      this.parent = parent;
      this.size = round(parent.buckets.size * MULTIPLIER_NUM_BUCKETS):int;
      this.bucketsDom = {0..#round(parent.buckets.size * MULTIPLIER_NUM_BUCKETS):int};
    }

    // _gen_key will generate the hash on the combined seed and hash of original key
    // which ensures a better distribution of keys from varying seeds.
    proc hash(key : keyType) {
      return _gen_key(chpl__defaultHashCombine(chpl__defaultHash(key), seed, 1));
    }

    proc _hash(key) {
      return _gen_key(chpl__defaultHashCombine(key, seed, 1));
    }

    proc releaseLock() {
      if (lock.read() == P_LOCK) then lock.write(P_TERM);
    }

    // proc size return buckets.size;
  }

  class ConcurrentMap : Base {
    var root : unmanaged Buckets(keyType, valType);
    var _manager = new owned LocalEpochManager();
    var iterRNG = new owned RandomStream(uint(64), parSafe=true);
    type stackType = (unmanaged Buckets(keyType, valType)?, int, int);
    type deferredType = (unmanaged Buckets(keyType, valType)?, int);
    type PEListType = (unmanaged Bucket(keyType, valType)?, unmanaged Buckets(keyType, valType)?, int);

    proc init(type keyType, type valType) {
      super.init(keyType, valType);
      root = new unmanaged Buckets(keyType, valType);
      root.lock.write(P_INNER);
    }

    proc getToken() : owned TokenWrapper {
      return _manager.register();
    }

    proc getEList(key : keyType, isInsertion : bool, tok : owned TokenWrapper) : unmanaged Bucket(keyType, valType)? {
      var found : unmanaged Bucket(keyType, valType)?;
      var curr = root;
      var shouldYield = false;
      const defaultHash = chpl__defaultHash(key);
      var idx = (curr._hash(defaultHash) % (curr.buckets.size):uint):int;
      while (true) {
        var next = curr.buckets[idx].read();
        // writeln("stuck");
        if (next == nil) {
          // If we're not inserting something, I.E we are removing 
          // or retreiving, we are done.
          if !isInsertion then return nil;

          // Otherwise, speculatively create a new bucket to add in.
          var newList = new unmanaged Bucket(curr);
          newList.lock.write(E_LOCK);

          // We set our Bucket, we also own it so return it
          if (curr.buckets[idx].compareAndSwap(nil, newList)) {
            return newList;
          } else {
            // Someone else set their bucket, reload.
            delete newList;
          }
        }
        else if (next!.lock.read() == P_INNER) {
          curr = next : unmanaged Buckets(keyType, valType);
          idx = (curr._hash(defaultHash) % (curr.buckets.size):uint):int;
        }
        else if (next!.lock.read() == E_AVAIL) {
          // We now own the bucket...
          if (next!.lock.compareAndSwap(E_AVAIL, E_LOCK)) {
            // Non-insertions don't care.
            if !isInsertion then return next : unmanaged Bucket(keyType, valType);
            // Insertions cannot have a full bucket...
            // If it is not full return it
            var bucket = next : unmanaged Bucket(keyType, valType)?;
            if bucket!.count < BUCKET_NUM_ELEMS then
              return bucket;

            for k in bucket!.keys {
              if k == key {
                return bucket;
              }
            }

            // writeln(bucket.count);
            // Rehash into new Buckets
            var newBuckets = new unmanaged Buckets(curr);
            for (k,v) in zip(bucket!.keys, bucket!.values) {
              var idx = (newBuckets.hash(k) % newBuckets.size:uint):int;
              if newBuckets.buckets[idx].read() == nil {
                newBuckets.buckets[idx].write(new unmanaged Bucket(newBuckets));
              }
              var buck = newBuckets.buckets[idx].read() : unmanaged Bucket(keyType, valType)?;
              buck!.count += 1;
              buck!.keys[buck!.count] = k;
              buck!.values[buck!.count] = v;
            }

            // TODO: Need to pass this to 'EpochManager.deferDelete'
            next!.lock.write(GARBAGE);
            tok.deferDelete(next);
            curr.buckets[idx].write(newBuckets: unmanaged Base(keyType, valType));
            curr = newBuckets;
            idx = (curr._hash(defaultHash) % (curr.buckets.size):uint):int;
          }
        }

        if shouldYield then chpl_task_yield(); // If lock could not be acquired
        shouldYield = true;
      }
      return nil;
    }

    // temporary function to facilitate deletion of EList
    proc getPEList(key : keyType, isInsertion : bool, tok : owned TokenWrapper) : PEListType {
      var found : unmanaged Bucket(keyType, valType)?;
      var retNil : PEListType;
      var curr = root;
      var shouldYield = false;
      const defaultHash = chpl__defaultHash(key);
      var idx = (curr._hash(defaultHash) % (curr.buckets.size):uint):int;
      while (true) {
        var next = curr.buckets[idx].read();
        // writeln("stuck");
        if (next == nil) {
          // If we're not inserting something, I.E we are removing 
          // or retreiving, we are done.
          if !isInsertion then return retNil;

          // Otherwise, speculatively create a new bucket to add in.
          var newList = new unmanaged Bucket(curr);
          newList.lock.write(E_LOCK);

          // We set our Bucket, we also own it so return it
          if (curr.buckets[idx].compareAndSwap(nil, newList)) {
            return (newList, curr, idx);
          } else {
            // Someone else set their bucket, reload.
            delete newList;
          }
        }
        else if (next!.lock.read() == P_INNER) {
          curr = next : unmanaged Buckets(keyType, valType);
          idx = (curr._hash(defaultHash) % (curr.buckets.size):uint):int;
        }
        else if (next!.lock.read() == E_AVAIL) {
          // We now own the bucket...
          if (next!.lock.compareAndSwap(E_AVAIL, E_LOCK)) {
            // Non-insertions don't care.
            if !isInsertion then return (next : unmanaged Bucket(keyType, valType), curr, idx);
            // Insertions cannot have a full bucket...
            // If it is not full return it
            var bucket = next : unmanaged Bucket(keyType, valType)?;
            if bucket!.count < BUCKET_NUM_ELEMS then
              return (bucket, curr, idx);

            for k in bucket!.keys {
              if k == key {
                return (bucket, curr, idx);
              }
            }

            // writeln(bucket.count);
            // Rehash into new Buckets
            var newBuckets = new unmanaged Buckets(curr);
            for (k,v) in zip(bucket!.keys, bucket!.values) {
              var idx = (newBuckets.hash(k) % newBuckets.size:uint):int;
              if newBuckets.buckets[idx].read() == nil {
                newBuckets.buckets[idx].write(new unmanaged Bucket(newBuckets));
              }
              var buck = newBuckets.buckets[idx].read() : unmanaged Bucket(keyType, valType)?;
              buck!.count += 1;
              buck!.keys[buck!.count] = k;
              buck!.values[buck!.count] = v;
            }

            // TODO: Need to pass this to 'EpochManager.deferDelete'
            next!.lock.write(GARBAGE);
            tok.deferDelete(next);
            curr.buckets[idx].write(newBuckets: unmanaged Base(keyType, valType));
            curr = newBuckets;
            idx = (curr._hash(defaultHash) % (curr.buckets.size):uint):int;
          }
        }

        if shouldYield then chpl_task_yield(); // If lock could not be acquired
        shouldYield = true;
      }
      return retNil;
    }

    // TODO: RAII based Locks.
    // Current iterator can be locked indefinitely if function breaks
    // Eg: for i in map do break;
    iter these() : (keyType, valType) {
      var tok = getToken();
      tok.pin();
      // Data stored on recursionStack: PointerList, start, i
      var recursionStack = new LockFreeStack(stackType);
      var deferred : unmanaged DeferredNode(deferredType)?;
      var restore = true;
      var curr : unmanaged Buckets(keyType, valType)? = root;
      var start = ((iterRNG.getNext())%(curr!.buckets.size):uint):int;
      var startIndex = 0;

      while (true) {
        restore = true;
        for i in startIndex..(curr!.buckets.size-1) {
          var idx = (start + i)%curr!.buckets.size;
          var bucketBase = curr!.buckets[idx].read();
          if (bucketBase != nil) {
            if (bucketBase!.lock.read() == E_AVAIL && bucketBase!.lock.compareAndSwap(E_AVAIL, E_LOCK)) {
              var bucket = bucketBase : unmanaged Bucket(keyType, valType)?;
              for j in 0..#bucket!.count do yield (bucket!.keys[j], bucket!.values[j]);
              bucket!.lock.write(E_AVAIL);
            } else if (bucketBase!.lock.read() == P_INNER) {
              var stackElem = (curr, start, i);
              recursionStack.push(stackElem);
              curr = bucketBase : unmanaged Buckets(keyType, valType)?;
              start = ((iterRNG.getNext())%(curr!.buckets.size):uint):int;
              startIndex = 0;
              restore = false;
              break;
            } else {
              var deferredElem = (curr, idx);
              var deferredNode = new unmanaged DeferredNode(deferredElem);
              deferredNode.next = deferred;
              deferred!.prev = deferredNode;
              deferred = deferredNode;
            }
          }
        }

        if (restore == true) {
        var (hasState, state) = recursionStack.pop();
          if (hasState) {
            curr = state[0];
            start = state[1];
            startIndex = state[2] + 1;
            continue;
          } else {
            var head = deferred;
            var continueFlag = false;
            while (head != nil) {
              var pList = head!.val[0];
              var idx = head!.val[1];
              var bucketBase = pList!.buckets[idx].read();
              var next = head!.next;
              if (bucketBase!.lock.read() == P_INNER) {
                delete head;
                curr = bucketBase : unmanaged Buckets(keyType, valType)?;
                start = ((iterRNG.getNext())%(curr!.size):uint):int;
                startIndex = 0;
                continueFlag = true;
                break;
              } else if (bucketBase!.lock.read() == E_AVAIL && bucketBase!.lock.compareAndSwap(E_AVAIL, E_LOCK)) {
                delete head;
                var bucket = bucketBase : unmanaged Bucket(keyType, valType)?;
                for j in 0..#bucket!.count do yield (bucket!.keys[j], bucket!.values[j]);
                bucket!.lock.write(E_AVAIL);
              }
              head = next;
            }

            if (continueFlag == false && deferred != nil) {
              chpl_task_yield();
            } else if (deferred == nil) then break;
          }
        }
      }
      tok.unpin();
    }

    iter these(param tag:iterKind) where tag == iterKind.standalone {
      var tok = getToken();
      tok.pin();
      var workList = new LockFreeQueue(unmanaged Buckets(keyType, valType)?);
      var _workListTok : owned TokenWrapper = workList.getToken();
      var deferredList = new LockFreeQueue(deferredType);
      var _deferredListTok : owned TokenWrapper = deferredList.getToken();
      var _startIdx = ((iterRNG.getNext())%(root.buckets.size):uint):int;
      var started : chpl__processorAtomicType(int);
      var finished : chpl__processorAtomicType(int);

      for i in 0..(root.buckets.size-1) {
        var idx = (_startIdx + i)%root.buckets.size;
        var bucketBase = root.buckets[idx].read();
        if (bucketBase != nil) {
          if (bucketBase!.lock.read() == E_AVAIL && bucketBase!.lock.compareAndSwap(E_AVAIL, E_LOCK)) {
            var bucket = bucketBase : unmanaged Bucket(keyType, valType)?;
            for j in 0..#bucket!.count do yield (bucket!.keys[j], bucket!.values[j]);
            bucket!.lock.write(E_AVAIL);
          } else if (bucketBase!.lock.read() == P_INNER) {
            var bucket = bucketBase : unmanaged Buckets(keyType, valType)?;
            started.add(1);
            workList.enqueue(bucket, _workListTok);
          } else {
            var deferredElem = (root, idx);
            deferredList.enqueue(deferredElem, _deferredListTok);
          }
        }
      }

      coforall tid in 1..here.maxTaskPar {
        var workListTok : owned TokenWrapper = workList.getToken();
        var deferredListTok : owned TokenWrapper = deferredList.getToken();
        while (true) {
          var (hasNode, _node) = workList.dequeue(workListTok);
          if (!hasNode) {
            var (hasDeferredNode, deferredNode) = deferredList.dequeue(deferredListTok);
            if (!hasDeferredNode) {
              if (started.read() == finished.read()) then break;
              else continue;
            }
            var pList = deferredNode[0];
            var idx = deferredNode[1];
            var bucketBase = pList!.buckets[idx].read();
            if (bucketBase != nil) {
              if (bucketBase!.lock.read() == E_AVAIL && bucketBase!.lock.compareAndSwap(E_AVAIL, E_LOCK)) {
                var bucket = bucketBase : unmanaged Bucket(keyType, valType)?;
                for j in 0..#bucket!.count do yield (bucket!.keys[j], bucket!.values[j]);
                bucket!.lock.write(E_AVAIL);
                continue;
              } else if (bucketBase!.lock.read() == P_INNER) {
                _node = bucketBase : unmanaged Buckets(keyType, valType)?;
              } else {
                deferredList.enqueue(deferredNode, deferredListTok);
                continue;
              }
            }
          } else finished.add(1);

          var startIdx = ((iterRNG.getNext())%(_node!.buckets.size):uint):int;
          for i in 0..(_node!.buckets.size-1) {
            var idx = (startIdx + i)%_node!.buckets.size;
            var bucketBase = _node!.buckets[idx].read();
            if (bucketBase != nil) {
              if (bucketBase!.lock.read() == E_AVAIL && bucketBase!.lock.compareAndSwap(E_AVAIL, E_LOCK)) {
                var bucket = bucketBase : unmanaged Bucket(keyType, valType)?;
                for j in 0..#bucket!.count do yield (bucket!.keys[j], bucket!.values[j]);
                bucket!.lock.write(E_AVAIL);
              } else if (bucketBase!.lock.read() == P_INNER) {
                var bucket = bucketBase : unmanaged Buckets(keyType, valType)?;
                started.add(1);
                workList.enqueue(bucket, workListTok);
              } else {
                var deferredElem = (_node, idx);
                deferredList.enqueue(deferredElem, deferredListTok);
              }
            }
          }
        }
      }

      tok.unpin();
    }

    proc insert(key : keyType, val : valType, tok : owned TokenWrapper = getToken()) : bool {
      tok.pin();
      var elist = getEList(key, true, tok);
      for i in 0..#elist!.count {
        if (elist!.keys[i] == key) {
          elist!.lock.write(E_AVAIL);
          tok.unpin();
          return false;
        }
      }
      elist!.count += 1;
      elist!.keys[elist!.count-1] = key;
      elist!.values[elist!.count-1] = val;
      elist!.lock.write(E_AVAIL);
      tok.unpin();
      return true;
    }

    proc find(key : keyType, tok : owned TokenWrapper = getToken()) : (bool, valType) {
      tok.pin();
      var elist = getEList(key, false, tok);
      var res : valType;
      if (elist == nil) then return (false, res);
      var found = false;
      for i in 0..#elist!.count {
        if (elist!.keys[i] == key) {
          res = elist!.values[i];
          found = true;
          break;
        }
      }
      elist!.lock.write(E_AVAIL);
      tok.unpin();
      return (found, res);
    }

    /*
      Returns `true` if the given key is a member of this map, and `false`
      otherwise.
      :arg key: The key to test for membership.
      :type key: keyType
      :arg tok: Token for EpochManager
      :returns: Whether or not the given key is a member of this map.
      :rtype: `bool`
    */
    proc const contains(const key : keyType, tok : owned TokenWrapper = getToken()) : bool {
      var (found, res) = find(key, tok);
      return found;
    }

    /* If the map doesn't contain a value at position `k` add one and
       set it to `v`. If the map already contains a value at position
       `k`, update it to the value `v`.
     */
    proc addOrSet(key: keyType, val: valType, tok : owned TokenWrapper = getToken()) {
      tok.pin();
      var elist = getEList(key, true, tok);
      for i in 0..#elist!.count {
        if (elist!.keys[i] == key) {
          elist!.values[i] = val;
          elist!.lock.write(E_AVAIL);
          tok.unpin();
          return;
        }
      }
      elist!.count += 1;
      elist!.keys[elist!.count-1] = key;
      elist!.values[elist!.count-1] = val;
      elist!.lock.write(E_AVAIL);
      tok.unpin();
    }

    proc erase(key : keyType, tok : owned TokenWrapper = getToken()) : bool {
      tok.pin();
      var (elist, pList, idx) = getPEList(key, false, tok);
      if (elist == nil) then return false;
      var res = false;
      for i in 0..#elist!.count {
        if (elist!.keys[i] == key) {
          elist!.keys[i] = elist!.keys[elist!.count-1];
          elist!.values[i] = elist!.values[elist!.count-1];
          elist!.count -= 1;
          res = true;
          break;
        }
      }

      if elist!.count == 0 {
        pList!.buckets[idx].write(nil);
        elist!.lock.write(GARBAGE);
        tok.deferDelete(elist);
      } else elist!.lock.write(E_AVAIL);
      tok.unpin();
      return res;
    }

    proc tryReclaim() {
      _manager.tryReclaim();
    }
  }

  config const N = 1024;
  use Time;

  proc randomOpsBenchmark (maxLimit : uint = max(uint(16))) {
    // Use map as an integer-based set
    writeln("Random operations test: ");
    var timer = new Timer();
    var map = new ConcurrentMap(int, int);
    map.insert(1..(maxLimit:int), 0);
    timer.start();
    coforall tid in 1..here.maxTaskPar {
      var tok = map.getToken();
      var rng = new RandomStream(real);
      var keyRng = new RandomStream(int);
      for i in 1..N {
        var s = rng.getNext();
        var key = keyRng.getNext(0, maxLimit:int);
        if s < 0.5 {
          map.insert(key,i,tok);
        } else {
          map.erase(key, tok);
        }
      }
    }
    timer.stop();
    writeln("Concurrent Map: ", timer.elapsed());
    timer.clear();
    writeln();
  }


  proc randomOpsStrongBenchmark (maxLimit : uint = max(uint(16))) {
    var timer = new Timer();
    var map = new ConcurrentMap(int, int);
    map.insert(1..65536, 0);
    var insertTime : atomic real;
    var eraseTime : atomic real;
    var insertCount : chpl__processorAtomicType(int);
    var eraseCount : chpl__processorAtomicType(int);
    var findTime : atomic real;
    var findCount : chpl__processorAtomicType(int);
    timer.start();
    coforall tid in 1..here.maxTaskPar {
    var rng = new RandomStream(real);
    var keyRng = new RandomStream(int);
    const opspertask = N / here.maxTaskPar;
    var tok = map.getToken();
      var timer1 = new Timer();
    for i in 1..opspertask {
      var s = rng.getNext();
      var key = keyRng.getNext(0, maxLimit:int);
      timer1.clear();
      if s < 0.10 {
        timer1.start();
            map.insert(key,i, tok);
            timer1.stop();
            insertTime.add(timer1.elapsed());
            insertCount.add(1);
      } else if s < 0.20 {
        timer1.start();
            map.erase(key, tok);
            timer1.stop();
            eraseTime.add(timer1.elapsed());
            eraseCount.add(1);
      } else {
        timer1.start();
            map.find(key, tok);
            timer1.stop();
            findTime.add(timer1.elapsed());
            findCount.add(1);
      }
    }
      // timer1.stop();
      // writeln(opsperloc / timer1.elapsed());
    }
    timer.stop();
    writeln("Insert average Time: ", insertTime.read()*10**9/insertCount.read());
      writeln("Erase average Time: ", eraseTime.read()*10**9/eraseCount.read());
      writeln("Find average Time: ", findTime.read()*10**9/findCount.read());
    writeln("Time taken : ", timer.elapsed());
    var opspersec = N/timer.elapsed();
    writeln("Completed ", N, " operations in ", timer.elapsed(), "s with ", opspersec, " operations/sec");
  }

  proc iterationBenchmark() {
    writeln("Iteration Test: ");
    var map = new ConcurrentMap(int, int);
    forall i in 0..65535 do map.insert(i, 0);
    var timer = new Timer();
    timer.start();
    forall i in map {
      sleep(10, TimeUnits.microseconds);
    }
    timer.stop();
    writeln("Concurrent iteration: " + timer.elapsed():string);
    timer.clear();

    timer.start();
    for i in map {
      sleep(10, TimeUnits.microseconds);
    }
    timer.stop();
    writeln("Serial iteration: " + timer.elapsed():string);
    timer.clear();
    writeln();
  }

  proc insertOpStrongBenchmark (maxLimit : uint = max(uint(16)), tasks = here.maxTaskPar) {
    var timer = new Timer();
    var map = new ConcurrentMap(int, int);
    timer.start();
    const opspertask = N / tasks;
    coforall tid in 1..tasks {
      var keyRng = new RandomStream(int);
      var tok = map.getToken();
      for i in 1..opspertask {
        var key = keyRng.getNext(0, maxLimit:int);
        map.insert(key,i,tok);
      }
    }
    timer.stop();
    const totalOps = opspertask*tasks;
    writeln(tasks, " tasks, ", ((10**9)*timer.elapsed())/totalOps, " ns/op");
  }

  proc eraseOpStrongBenchmark (maxLimit : uint = max(uint(16)), tasks = here.maxTaskPar) {
    var timer = new Timer();
    var map = new ConcurrentMap(int, int);
    forall i in 0..65535 do map.insert(i, 0);
    timer.start();
    const opspertask = N / tasks;
    coforall tid in 1..tasks {
      var keyRng = new RandomStream(int);
      var tok = map.getToken();
      for i in 1..opspertask {
        var key = keyRng.getNext(0, maxLimit:int);
        map.erase(key,tok);
      }
    }
    timer.stop();
    const totalOps = opspertask*tasks;
    writeln(tasks, " tasks, ", ((10**9)*timer.elapsed())/totalOps, " ns/op");
  }

  proc reclaimBenchmark (maxLimit : uint = max(uint(16)), tasks = here.maxTaskPar) {
    var map = new ConcurrentMap(int, int);
    writeln(memoryUsed());
    forall i in 0..65535 with (var tok = map.getToken()) { map.insert(i, 0, tok); }
    writeln(memoryUsed());
    forall i in 0..65535 with (var tok = map.getToken()) { map.erase(i, tok); }
    writeln(memoryUsed());
    map.tryReclaim();
    map.tryReclaim();
    map.tryReclaim();
    writeln(memoryUsed());
  }

  proc findOpStrongBenchmark (maxLimit : uint = max(uint(16)), tasks = here.maxTaskPar) {
    var timer = new Timer();
    var map = new ConcurrentMap(int, int);
    forall i in 0..65535 do map.insert(i, 0);
    timer.start();
    const opspertask = N / tasks;
    coforall tid in 1..tasks {
      var keyRng = new RandomStream(int);
      var tok = map.getToken();
      for i in 1..opspertask {
        var key = keyRng.getNext(0, maxLimit:int);
        map.find(key,tok);
      }
    }
    timer.stop();
    const totalOps = opspertask*tasks;
    writeln(tasks, " tasks, ", ((10**9)*timer.elapsed())/totalOps, " ns/op");
  }

  proc intSetStrongBenchmark (maxLimit : uint = max(uint(16)), tasks = here.maxTaskPar) {
    var timer = new Timer();
    var map = new ConcurrentMap(int, int);
    forall i in 0..65535 do map.insert(i, 0);
    timer.start();
    const opspertask = N / tasks;
    coforall tid in 1..tasks {
      var rng = new RandomStream(real);
      var keyRng = new RandomStream(int);
      var tok = map.getToken();
      for i in 1..opspertask {
        var s = rng.getNext();
        var key = keyRng.getNext(0, maxLimit:int);
        if s < 0.8 {
          map.find(key, tok);
        } else if s < 0.9 {
          map.insert(key, i, tok);
        } else {
          map.erase(key, tok);
        }
      }
    }
    timer.stop();
    const totalOps = opspertask*tasks;
    writeln(tasks, " tasks, ", ((10**9)*timer.elapsed())/totalOps, " ns/op");
  }

  proc main() {
    var map = new ConcurrentMap(int, int);
    forall i in 1..N with (var tok = map.getToken()) {
      map.addOrSet(i, i**2, tok);
    }
    for (key,val) in map {
      writeln(key, " ", val);
    }

    forall i in 1..(N+5) with (var tok = map.getToken()) {
      // writeln(i, map.contains(i, tok));
      map.addOrSet(i, 1);
    }
    for (key,val) in map {
      writeln(key, " ", val);
    }
  }
}
