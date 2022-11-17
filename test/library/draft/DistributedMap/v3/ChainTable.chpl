
// A fork of ChapelHashtable that uses chaining to resolve collisions instead of linear probing

module ChainTable {

    use ChapelBase, DSIUtil;
    private use CTypes;

    const defaultInitialCapacity : uint = 32;
    param numLocalBucketSlots : uint = 2;

    // ----------------------------------------------------
    // Component data structures for Bucket implementation
    // ----------------------------------------------------

    // empty needs to be 0 so memset 0 sets it
    enum entryStatus { empty=0, full, deleted };

    // an entry in the hash table
    record rawTableEntry {
        type keyType;
        type valType;

        var key: keyType;
        var val: valType;

        var status: entryStatus = entryStatus.empty;
        inline proc isFull() {
            return this.status == entryStatus.full;
        }
    }

    // a linked-list node which holds a hashtable entry
    class ChainedTableEntry {
        type keyType;
        type valType;
        var entry : rawTableEntry(keyType, valType);
        var next : owned ChainedTableEntry(keyType, valType)?;

        proc appendToTail(idx: uint) : uint {
            if this.next != nil {
                return this.next!.appendToTail(idx + 1);
            } else {
                this.next = new owned ChainedTableEntry(keyType, valType);
                return idx;
            }
        }
    }

    enum bucketInitState { uninit=0, initialized };

    // a bucket for holding hashtable entries that all correspond the the same hash
    //
    // some small number of entries are stored in a local tuple, and the remainder
    //  are stored on the heap in a linked list
    //
    // this type provides an interface for finding and modifying entries in the
    //  bucket. Modifications are done by reference using the 'this()' accessor
    record Bucket {
        type keyType;
        type valType;

        // store some number of local entries to reduce # of cache misses when load factor is low
        var localEntries: numLocalBucketSlots*rawTableEntry(keyType, valType);

        // store the remainder of the entries in a linked list on the heap
        var chainHead: owned ChainedTableEntry(keyType, valType)?;

        var initState : bucketInitState = bucketInitState.uninit;
        inline proc isInit() {
            return this.initState == bucketInitState.initialized;
        }

        proc init(type keyType, type valType) {
            this.keyType = keyType;
            this.valType = valType;
            this.chainHead = nil;
            this.initState = bucketInitState.initialized;
        }

        proc clear() {
            for i in 0..numLocalBucketSlots do this.localEntries[i].status = entryStatus.deleted;
            delete this.chainHead;
        }

        // determine if this bucket has a 'full' entry with the given key.
        //  if so, return (true, idx of the slot)
        //  otherwise, return (false, 0)
        proc findIndexOf(key: keyType) : (bool, uint) {
            for (e, idx) in zip(this._allEntries(), 0..) {
                if e.isFull() && keysMatch(e.key, key) then return (true, idx);
            }
            return (false, 0);
        }

        // return the index of the next open slot
        proc firstAvailableIndex() : uint {
            for (e, idx) in zip(this._allEntries(), 0..) {
                if !e.isFull() then return idx;
            }
            // none of the existing slots are empty or deleted,
            //  so add one to the end of the linked list and return its index
            if this.chainHead == nil {
                this.chainHead = new owned ChainedTableEntry(this.keyType, this.valType);
                return numLocalBucketSlots;
            } else {
                return this.chainHead!.appendToTail(numLocalBucketSlots + 1);
            }
        }

        // return a reference to the entry at the given index
        proc this(idx: uint) ref {
            if idx < numLocalBucketSlots {
                ref entry = this.localEntries[idx];
                return entry;
            } else {
                var cnt = numLocalBucketSlots;
                var h = this.chainHead.borrow();
                while h != nil {
                    if cnt == idx {
                        ref entry = h!.entry;
                        return entry;
                    }
                    h = h!.next.borrow();
                    cnt += 1;
                }
                halt("Bucket index out of bounds!");
            }
        }

        // Iterate over of all of the Bucket's entries
        //  yield the local entries first, then those in the linked list
        iter _allEntries() ref : rawTableEntry(this.keyType, this.valType) {
            for i in 0..<numLocalBucketSlots {
                ref entry = this.localEntries[i];
                yield entry;
            }

            var h = this.chainHead.borrow();
            while h != nil {
                ref entry = h!.entry;
                yield entry;
                h = h!.next.borrow(); // could be nil
            }
        }
    }

    // --------------------------
    // Hash Table Implementation
    // --------------------------

    record chainTable {
        type keyType;
        type valType;

        var numBuckets : uint;
        var numEntries : uint;

        var rehashing : atomic bool;
        var numStaticManagers: atomic uint;

        var buckets: _ddata(Bucket(keyType, valType));

        proc init(type keyType, type valType, initialCapacity = defaultInitialCapacity) {
            if isDomainType(keyType) then
                compilerError("Values of 'domain' type do not support hash functions yet", 2);

            this.keyType = keyType;
            this.valType = valType;
            this.numBuckets = initialCapacity / numLocalBucketSlots;
            this.numEntries = 0;

            this.buckets = (if this.numBuckets == 0
                then nil
                else _allocateData(this.numBuckets, Bucket(this.keyType, this.valType))
            );
        }

        proc deinit() {
            if this.buckets != nil {
                _ddata_free(this.buckets, this.numBuckets);
            }
        }

        // determine if the map has an entry for the given key
        //  if so, return (true, bucket index, index within the bucket)
        //  otherwise, return (false, bucket index, 0)
        proc getFullSlotFor(key: keyType) : (bool, (uint, uint)) {
            var bucket_idx = this._bucketIdx(key);
            var (has_key, chain_idx) = this.buckets[bucket_idx].findIndexOf(key);
            return (has_key, (bucket_idx, chain_idx));
        }

        // return the indices for a given key
        //  if an entry is already present for the key, return (bucket index, its index within the bucket)
        //  otherwise, return (bucket index, next open index in the bucket)
        proc getSlotFor(key: keyType) : (uint, uint) {
            var bucket_idx = this._bucketIdx(key);
            var (has_key, chain_idx) = this.buckets[bucket_idx].findIndexOf(key);
            return if has_key
                then (bucket_idx, chain_idx)
                else (bucket_idx, this.buckets[bucket_idx].firstAvailableIndex());
        }

        // copy a key-value pair into a slot with the given indices
        proc fillSlot((bucket_idx, chain_idx): (uint, uint), in key: keyType, in val: valType) {
            use Memory.Initialization;

            ref entry = this.buckets[bucket_idx][chain_idx];

            select entry.status {
                when entryStatus.full do _deinitSlot(entry);
                when entryStatus.empty do this.numEntries += 1;
                when entryStatus.deleted do this.numEntries += 1;
            }

            entry.status = entryStatus.full;
            moveInitialize(entry.key, key);
            moveInitialize(entry.val, val);
        }

        // copy a key-value pair out of a slot with the given indices
        proc remove((bucket_idx, chain_idx): (uint, uint), out key: keyType, out val: valType) {
            use Memory.Initialization;

            ref entry = this.buckets[bucket_idx][chain_idx];

            key = moveToValue(entry.key);
            val = moveToValue(entry.val);
            entry.status = entryStatus.deleted;

            this.numEntries -= 1;
        }

        proc clear() {
            for i in 0..this.numBuckets {
                this.buckets[i].clear();
            }
        }

        // replace the current array of buckets with a new array of different size
        proc _rehash(newNumBuckets: uint) {
            if newNumBuckets == 0 then halt("cannot rehash into zero buckets");
            this.rehashing.write(true); defer this.rehashing.write(false);

            const oldBuckets = this.buckets;
            const oldNumBuckets = this.numBuckets;

            this.buckets = _allocateData(this.numBuckets, Bucket(this.keyType, this.valType));
            this.numBuckets = newNumBuckets;

            for i in 0..oldNumBuckets {
                ref b = oldBuckets[i];
                for e in b._allEntries() {
                    this.fillSlot(this.getSlotFor(e.key), e.key, e.val);
                }
            }

            if oldBuckets != nil then _ddata_free(oldBuckets, oldNumBuckets);
        }

        proc _bucketIdx(key: keyType) : uint {
            const idx = chpl__defaultHashWrapper(key):uint & (this.numBuckets - 1);
            if !this.buckets[idx].isInit() then this.buckets[idx] = new Bucket(this.keyType, this.valType);
            return idx;
        }

        inline proc _loadFactor() : real {
            return (this.numEntries : real) / ((this.numBuckets * numLocalBucketSlots): real);
        }
    }

    private proc keysMatch(key1: ?t, key2: t) {
      if isArrayType(key2.type) {
        return (key1.equals(key2));
      } else {
        return key1 == key2;
      }
    }

    // ------------------------
    // Memory Helper Functions
    // ------------------------

    private proc _allocateData(size:uint, type tableEltType) {
        if size == 0 then
            halt("attempt to allocate hashtable with size 0");

        var callPostAlloc: bool;
        var ret = _ddata_allocate_noinit(tableEltType,
                                        size,
                                        callPostAlloc);

        var initMethod = init_elts_method(size, tableEltType);

        const sizeofElement = _ddata_sizeof_element(ret);

        // The memset call below needs to be able to set _array records.
        // But c_ptrTo on an _array will return a pointer to
        // the first element, which messes up the shallowCopy/shallowSwap code
        //
        // As a workaround, this function just returns a pointer to the argument,
        // whether or not it is an array.
        inline proc ptrTo(ref x) {
            return c_pointer_return(x);
        }

        select initMethod {
            when ArrayInit.noInit {
                // do nothing
            }
            when ArrayInit.serialInit {
                for slot in 0..#size {
                    c_memset(ptrTo(ret[slot]), 0:uint(8), sizeofElement);
                }
            }
            when ArrayInit.parallelInit {
                // This should match the 'these' iterator in terms of idx->task
                forall slot in 0..#size {
                    c_memset(ptrTo(ret[slot]), 0:uint(8), sizeofElement);
                }
            }
            otherwise {
                halt("ArrayInit.heuristicInit should have been made concrete");
            }
        }

        if callPostAlloc {
            _ddata_allocate_postalloc(ret, size);
        }

        return ret;
    }

    private proc _typeNeedsDeinit(type t) param {
        return __primitive("needs auto destroy", t);
    }
    private proc _deinitSlot(ref aSlot: rawTableEntry) {
        if _typeNeedsDeinit(aSlot.key.type) {
            chpl__autoDestroy(aSlot.key);
        }
        if _typeNeedsDeinit(aSlot.val.type) {
            chpl__autoDestroy(aSlot.val);
        }
    }
}
