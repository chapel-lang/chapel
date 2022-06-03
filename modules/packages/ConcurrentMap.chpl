/*
 * Copyright 2020-2022 Hewlett Packard Enterprise Development LP
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

/* This module provides a fast, scalable, fine-grained concurrent map.

  .. warning::

    This module relies on the :mod:`AtomicObjects` package module, which
    has several platform restrictions in its current state:

      - It relies on Chapel ``extern`` code blocks and so requires that
        the Chapel compiler is built with LLVM enabled.
      - Currently only ``CHPL_TARGET_ARCH=x86_64`` is supported as it uses
        the x86-64 instruction: CMPXCHG16B_.
      - The implementation relies on ``GCC`` style inline assembly, and so
        is restricted to a ``CHPL_TARGET_COMPILER`` value of ``gnu``,
        ``clang``, or ``llvm``.

    .. _CMPXCHG16B: https://www.felixcloutier.com/x86/cmpxchg8b:cmpxchg16b

  This module was
  inspired by the Interlocked Hash Table [#]_. It allows large critical
  sections that access a single table element, and can easily support multikey
  atomic operations. At the time of its development, ConcurrentMap outperformed
  Chapel's built-in map when used in a multithreaded mode by orders of
  magnitude.

  .. [#] L. Jenkins, T. Zhou and M. Spear, "Redesigning Go’s Built-In Map to
    Support Concurrent Operations," 2017 26th International Conference on
    Parallel Architectures and Compilation Techniques (PACT), 2017.
*/
module ConcurrentMap {
  private use AtomicObjects;
  private use LockFreeStack;
  private use LockFreeQueue;
  private use EpochManager;
  private use Random;
  private use IO;

  pragma "no doc"
  param BUCKET_UNLOCKED = 0;

  pragma "no doc"
  param BUCKET_LOCKED = 1;

  pragma "no doc"
  param BUCKET_DESTROYED = 2;

  /*
    Maximum number of key-value pairs stored in each leaf bucket.
  */
  config param BUCKET_NUM_ELEMS = 8;

  /*
    Maximum number of child buckets for the root bucket.
  */
  config const DEFAULT_NUM_BUCKETS = 1024;

  /*
    Multiplier for child bucket size.
    childBucketSize = MULTIPLIER_NUM_BUCKETS * parentBucketSize
  */
  config param MULTIPLIER_NUM_BUCKETS : real = 2;

  // Note: Once this becomes distributed, we have to make it per-locale
  pragma "no doc"
  var seedRNG = new owned RandomStream(uint(64), parSafe=true);

  pragma "no doc"
  const E_AVAIL = 1;

  pragma "no doc"
  const E_LOCK = 2;

  pragma "no doc"
  const P_INNER = 3;

  pragma "no doc"
  const P_TERM = 4;

  pragma "no doc"
  const P_LOCK = 5;

  pragma "no doc"
  const GARBAGE = 6;

  pragma "no doc"
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

  pragma "no doc"
  class StackNode {
    type eltType;
    var val : eltType;
    var next : unmanaged StackNode(eltType)?;

    proc init(type eltType) {
      this.eltType = eltType;
    }

    proc init(val : ?eltType) {
      this.eltType = eltType;
      this.val = val;
    }
  }

  pragma "no doc"
  class Stack {
    type eltType;
    var top : unmanaged StackNode(eltType)?;
    var count : int;

    proc init(type eltType) {
      this.eltType = eltType;
    }

    proc push(val : eltType) {
      var node = new unmanaged StackNode(val);
      node.next = top;
      top = node;
      count += 1;
    }

    proc pop() : eltType {
      if (count > 0) {
        var ret = top!.val;
        var next = top!.next;
        delete top;
        top = next;
        count -= 1;
        return ret;
      } else {
        var temp : eltType;
        return temp;
      }
    }

    proc isEmpty() : bool {
      return count == 0;
    }
  }

  pragma "no doc"
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

  pragma "no doc"
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

  pragma "no doc"
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
      return _gen_key(chpl__defaultHashCombine(key.hash(), seed, 1));
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
    pragma "no doc"
    var root : unmanaged Buckets(keyType, valType);

    pragma "no doc"
    var _manager = new owned LocalEpochManager();

    pragma "no doc"
    var iterRNG = new owned RandomStream(uint(64), parSafe=true);

    pragma "no doc"
    type stackType = (unmanaged Buckets(keyType, valType)?, int, int);

    pragma "no doc"
    type deferredType = (unmanaged Buckets(keyType, valType)?, int);

    pragma "no doc"
    type PEListType = (unmanaged Bucket(keyType, valType)?, unmanaged Buckets(keyType, valType)?, int);

    proc init(type keyType, type valType) {
      super.init(keyType, valType);
      root = new unmanaged Buckets(keyType, valType);
      root.lock.write(P_INNER);
    }

    /*
      Register the task to epoch manager.
    */
    proc getToken() : owned TokenWrapper {
      return _manager.register();
    }

    pragma "no doc"
    proc getEList(key : keyType, isInsertion : bool, tok : owned TokenWrapper) : unmanaged Bucket(keyType, valType)? throws {
      var found : unmanaged Bucket(keyType, valType)?;
      var curr = root;
      var shouldYield = false;
      const defaultHash = key.hash();
      var idx = (curr._hash(defaultHash) % (curr.buckets.size):uint):int;
      while (true) {
        var next = curr.buckets[idx].read();
        // writeln("stuck");
        if (next == nil) {
          // If we're not inserting something, I.E we are removing
          // or retrieving, we are done.
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

    // helper function to facilitate deletion of EList
    pragma "no doc"
    proc getPEList(key : keyType, isInsertion : bool, tok : owned TokenWrapper) : PEListType throws {
      var found : unmanaged Bucket(keyType, valType)?;
      var retNil : PEListType;
      var curr = root;
      var shouldYield = false;
      const defaultHash = key.hash();
      var idx = (curr._hash(defaultHash) % (curr.buckets.size):uint):int;
      while (true) {
        var next = curr.buckets[idx].read();
        // writeln("stuck");
        if (next == nil) {
          // If we're not inserting something, I.E we are removing
          // or retrieving, we are done.
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
    /*
      Serially iterates over the key-value pairs of this map.

      :yields: A tuple whose elements are a copy of one of the key-value
               pairs contained in this map.

      .. note::
        While iterating the map, pre-maturely breaking the iteration loop may
        permanently lock the map.
    */
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
              if (deferred != nil) {
                deferredNode.next = deferred;
                deferred!.prev = deferredNode;
              }
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

    /*
      Serially iterates over the key-value pairs of this map.

      :yields: A tuple whose elements are a copy of one of the key-value
               pairs contained in this map.
    */
    iter items() : (keyType, valType) {
      for i in this {
        yield i;
      }
    }

    /*
      Serially iterates over the keys of this map.

      :yields: A copy of one of the keys contained in this map.
    */
    iter keys() : keyType {
      for (key, val) in this {
        yield key;
      }
    }

    /*
      Serially iterates over the values of this map.

      :yields: A copy of one of the values contained in this map.
    */
    iter values() : valType {
      for (key, val) in this {
        yield val;
      }
    }

    /*
      Iterates over the key-value pairs of this map in parallel.

      :yields: A tuple whose elements are a copy of one of the key-value
               pairs contained in this map.
    */
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

    /*
      Parallelly iterates over the key-value pairs of this map.

      :yields: A tuple whose elements are a copy of one of the key-value
               pairs contained in this map.
    */
    iter items(param tag:iterKind) where tag == iterKind.standalone {
      forall i in this {
        yield i;
      }
    }

    /*
      Parallelly iterates over the keys of this map.

      :yields: A copy of one of the keys contained in this map.
    */
    iter keys(param tag:iterKind) where tag == iterKind.standalone {
      forall (key, val) in this {
        yield key;
      }
    }

    /*
      Parallelly iterates over the values of this map.

      :yields: A copy of one of the values contained in this map.
    */
    iter values(param tag:iterKind) where tag == iterKind.standalone {
      forall (key, val) in this {
        yield val;
      }
    }

    /*
      Adds a key-value pair to the map. Method returns `false` if the key
      already exists in the map.

     :arg key: The key to add to the map
     :type key: keyType

     :arg val: The value that maps to ``k``
     :type kal: valueType

     :arg tok: Token for EpochManager

     :returns: `true` if `key` was not in the map and added with value `val`.
               `false` otherwise.
     :rtype: bool
    */
    proc add(key : keyType, val : valType, tok : owned TokenWrapper = getToken()) : bool throws {
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

    /*
      Sets the value associated with a key. Method returns `false` if the key
      does not exist in the map.

     :arg key: The key whose value needs to change
     :type key: keyType

     :arg val: The desired value to the key ``key``
     :type val: valueType

     :arg tok: Token for EpochManager

     :returns: `true` if `key` was in the map and its value is updated with `val`.
               `false` otherwise.
     :rtype: bool
    */
    proc set(key: keyType, in val: valType, tok : owned TokenWrapper = getToken()): bool throws {
      tok.pin();
      var elist = getEList(key, false, tok);
      var res : valType;
      if (elist == nil) {
        tok.unpin();
        return false;
      }
      var found = false;
      for i in 0..#elist!.count {
        if (elist!.keys[i] == key) {
          elist!.values[i] = val;
          found = true;
          break;
        }
      }
      elist!.lock.write(E_AVAIL);
      tok.unpin();
      return found;
    }

    /* Get a copy of the element stored at position `key`.
     */
    proc getValue(key : keyType, tok : owned TokenWrapper = getToken()) : (bool, valType) throws {
      tok.pin();
      var elist = getEList(key, false, tok);
      var res : valType;
      if (elist == nil) {
        tok.unpin();
        return (false, res);
      }
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
    proc const contains(const key : keyType, tok : owned TokenWrapper = getToken()) : bool throws {
      var (found, res) = getValue(key, tok);
      return found;
    }

    /* If the map doesn't contain a value at position `k` add one and
       set it to `v`. If the map already contains a value at position
       `k`, update it to the value `v`.
     */
    proc addOrSet(key: keyType, val: valType, tok : owned TokenWrapper = getToken()) throws {
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

    /*
      Extends this map with the contents of the other, overwriting the values
      for already-existing keys.

      :arg m: The other map
    */
    proc extend(m : ConcurrentMap(keyType, valType)) throws {
      forall (key, value) in m with (var tok = getToken()) {
        addOrSet(key, value, tok);
      }
    }

    /* Remove the element at position `key` from the map and return its value
     */
    proc getAndRemove(key: keyType, tok : owned TokenWrapper = getToken()) throws {
      tok.pin();
      var (elist, pList, idx) = getPEList(key, false, tok);
      if (elist == nil) then halt("map index " + key:string + " out of bounds");
      var res : valType;
      var found = false;
      for i in 0..#elist!.count {
        if (elist!.keys[i] == key) {
          res = elist!.values[i];
          elist!.keys[i] = elist!.keys[elist!.count-1];
          elist!.values[i] = elist!.values[elist!.count-1];
          elist!.count -= 1;
          found = true;
          break;
        }
      }

      if elist!.count == 0 {
        pList!.buckets[idx].write(nil);
        elist!.lock.write(GARBAGE);
        tok.deferDelete(elist);
      } else elist!.lock.write(E_AVAIL);
      tok.unpin();

      if found then return res;
      else halt("map index " + key:string + " out of bounds");
    }

    /*
      Removes a key-value pair from the map, with the given key.

      :arg key: The key to remove from the map

      :arg tok: Token for EpochManager

      :returns: `false` if `key` was not in the map.  `true` if it was and removed.
      :rtype: bool
    */
    proc remove(key : keyType, tok : owned TokenWrapper = getToken()) : bool throws {
      tok.pin();
      var (elist, pList, idx) = getPEList(key, false, tok);
      if (elist == nil) {
        tok.unpin();
        return false;
      }
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

    pragma "no doc"
    proc clearHelper(curr : unmanaged Buckets(keyType, valType)?, tok : owned TokenWrapper) throws {
      var shouldYield = false;
      var idx = 0;

      while (idx < curr!.buckets.size) {
        var increment = false;
        var bucketBase = curr!.buckets[idx].read();

        if (bucketBase != nil) {
          if (bucketBase!.lock.read() == E_AVAIL && bucketBase!.lock.compareAndSwap(E_AVAIL, GARBAGE)) {
            curr!.buckets[idx].write(nil);
            var bucket = bucketBase : unmanaged Bucket(keyType, valType)?;
            tok.deferDelete(bucket);
            increment = true;
          } else if (bucketBase!.lock.read() == P_INNER) {
            var r = bucketBase : unmanaged Buckets(keyType, valType)?;
            clearHelper(r, tok);
            increment = true;
          } else {
            if shouldYield then chpl_task_yield(); // If lock could not be acquired
            shouldYield = true;
          }
        } else {
          increment = true;
        }

        if increment then idx += 1;
      }
    }

    /*
      Clears the contents of this map.
    */
    proc clearMap(tok : owned TokenWrapper = getToken()) throws {
      // var curr : unmanaged Buckets(keyType, valType)? = root;
      tok.pin();
      clearHelper(root, tok);
      tok.unpin();
    }

    /*
      Returns a new 0-based array containing a copy of key-value pairs as
      tuples.

      :return: A new DefaultRectangular array.
      :rtype: [] (keyType, valType)
    */
    proc toArray(): [] (keyType, valType) throws {
      type stackType = (keyType, valType);
      var stack = new Stack(stackType);
      for i in this {
        stack.push(i);
      }

      var size = stack.count;
      var A: [0..#size] (keyType, valType);

      for i in 0..#size {
        A[i] = stack.pop();
      }

      return A;
    }

    /*
      Trigger EpochManager to reclaim any reclaimable memory.
    */
    proc tryReclaim() {
      _manager.tryReclaim();
    }

    /*
      Returns a new 0-based array containing a copy of keys. Array is not
      guaranteed to be in any particular order.

      :return: A new DefaultRectangular array.
      :rtype: [] keyType
    */
    proc keysToArray(): [] keyType throws {
      var stack = new Stack(keyType);
      for (key, val) in this {
        stack.push(key);
      }

      var size = stack.count;
      var A: [0..#size] keyType;

      for i in 0..#size {
        A[i] = stack.pop();
      }

      return A;
    }

    /*
      Returns a new 0-based array containing a copy of values. Array is not
      guaranteed to be in any particular order.

      :return: A new DefaultRectangular array.
      :rtype: [] valType
    */
    proc valuesToArray(): [] valType throws {
      var stack = new Stack(valType);
      for (key, val) in this {
        stack.push(val);
      }

      var size = stack.count;
      var A: [0..#size] valType;

      for i in 0..#size {
        A[i] = stack.pop();
      }

      return A;
    }

    pragma "no doc"
    proc readThis(f) throws {
      compilerWarning("Reading a ConcurrentMap is not supported");
    }

    /*
      Writes the contents of this map to a channel. The format looks like:

        .. code-block:: chapel

           {k1: v1, k2: v2, .... , kn: vn}

      :arg ch: A channel to write to.
    */
    proc writeThis(f) throws {
      ch <~> "{";
      var first = true;
      for (key, val) in this {
        if first {
          ch <~> key <~> ": " <~> val;
          first = false;
        } else {
          ch <~> ", " <~> key <~> ": " <~> val;
        }
      }
      ch <~> "}";
    }

    /*
      Writes the contents of this map to a channel. The format looks like:

        .. code-block:: chapel

           {k1: v1, k2: v2, .... , kn: vn}

      :arg ch: A channel to write to.
    */
    deprecated "'readWriteThis' methods are deprecated. Use 'readThis' and 'writeThis' methods instead."
    proc readWriteThis(ch: channel) throws {
      if ch.writing then writeThis(ch);
      else readThis(ch);
    }
  }

  /*
    Replace the content of this map with the other's.

    :arg lhs: The map to assign to.
    :arg rhs: The map to assign from.
  */
  operator =(ref lhs: ConcurrentMap, const ref rhs: ConcurrentMap) {
    var tok = lhs.getToken();
    lhs.clearMap(tok);
    for (key, val) in rhs { // Can also feature a parallel addition
      lhs.add(key, val, tok);
    }
  }

  /*
    Returns `true` if the contents of two maps are the same.

    :arg a: A map to compare.
    :type a: map

    :arg b: A map to compare.
    :type b: map (with same keyType and valType)

    :return: `true` if the contents of two maps are equal.
    :rtype: `bool`
  */
  operator ==(const ref a: ConcurrentMap, const ref b: ConcurrentMap): bool throws {
    var atok = a.getToken();
    var btok = b.getToken();
    var result = true;
    for (key, val) in a {                   // Can also be done parallelly
      var (found, Val) = b.getValue(key, btok);
      if !found || val != Val then
        result = false;
    }
    if (!result) then return false;

    for (key, val) in b {
      var (found, Val) = a.getValue(key, atok);
      if !found || val != Val then
        result = false;
    }
    return result;
  }

    /*
    Returns `true` if the contents of two maps are not the same.

    :arg a: A map to compare.
    :type a: map

    :arg b: A map to compare.
    :type b: map (with same keyType and valType)

    :return: `true` if the contents of two maps are not equal.
    :rtype: `bool`
  */
  operator !=(const ref a: ConcurrentMap, const ref b: ConcurrentMap): bool throws {
    return !(a == b);
  }
}
