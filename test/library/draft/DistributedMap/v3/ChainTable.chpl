
// A fork of ChapelHashtable that uses chaining to resolve collisions instead of linear probing

module ChainTable {

    use ChapelBase, DSIUtil;
    private use CTypes;

    const defaultInitialCapacity : int = 32;
    param numLocalBucketSlots : uint = 2;

    // empty needs to be 0 so memset 0 sets it
    enum entryStatus { empty=0, full, deleted };

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

    class chainedTableEntry {
        type keyType;
        type valType;
        var entry : rawTableEntry(keyType, valType);
        var next : owned chainedTableEntry(keyType, valType)?;
    }

    class Bucket {
        type keyType;
        type valType;

        // store some number of local entries to reduce # of cache misses when load factor is low
        var localEntries: [0..<numLocalBucketSlots] rawTableEntry(keyType, valType);

        // store the remainder of the entries in a linked list on the heap
        var heapEntriesHead: owned chainedTableEntry(keyType, valType)?;

        var numEntries : int;

        proc init(type keyType, type valType) {
            this.keyType = keyType;
            this.valType = valType;
            this.complete();
            this.heapEntriesHead = nil;
            this.numEntries = 0;
        }

        proc findIndexOf(key: keyType) : (bool, uint) {
            for (e, idx) in this._allEntries() {
                if e.isFull() && keysMatch(e.key, key) then return (true, idx);
            }
            return (false, 0);
        }

        proc firstAvailableIndex() : uint {
            for (e, idx) in this._allEntries() {
                if !e.isFull() then return idx;
            }
            // none of the existing slots are empty or deleted,
            //  so add one to the linked list and return its index
            var slot_idx = numLocalBucketSlots;
            var h = this.heapEntriesHead.borrow();
            while h != nil {
                h = h!.next.borrow();
                slot_idx += 1;
            }
            h = new chainedTableEntry(this.keyType, this.valType);
            return slot_idx;
        }

        // Get an iterator over of all of the Bucket's entries
        //  yield the local entries first, then those linked on the heap
        iter _allEntries() ref : (rawTableEntry, uint) {
            var entry_idx: uint = 0;

            for e in this.localEntries {
                ref entry = e;
                yield (entry, entry_idx);
                entry_idx += 1;
            }

            var h = this.heapEntriesHead.borrow();
            while h != nil {
                ref entry = h!.entry;
                yield (entry, entry_idx);
                h = h!.next.borrow(); // could be nil
                entry_idx += 1;
            }
        }

        proc this(idx: uint) ref {
            if idx < numLocalBucketSlots {
                ref entry = this.localEntries[idx];
                return entry;
            } else {
                var cnt = numLocalBucketSlots;
                while h != nil {
                    if cnt == idx {
                        ref entry = h!.entry;
                        return entry;
                    }
                    h = h!.next.borrow();
                    cnt += 1;
                }
                halt("wut?");
            }
        }
    }

    record chainTable {
        type keyType;
        type valType;

        var numBuckets : int;
        var numEntries : int;

        var buckets: _ddata(unmanaged Bucket(keyType, valType));

        proc init(type keyType, type valType, initialCapacity = defaultInitialCapacity) {
            if isDomainType(keyType) then
                compilerError("Values of 'domain' type do not support hash functions yet", 2);

            this.keyType = keyType;
            this.valType = valType;
            this.numBuckets = initialCapacity / numLocalBucketSlots;
            this.numEntries = 0;

            // this.complete();
            this.buckets = (if this.numBuckets == 0
                then nil
                else _allocateData(this.numBuckets, unmanaged Bucket(this.keyType, this.valType))
            );
        }

        proc getFullSlotFor(key: keyType) : (bool, uint, uint) {
            var bucket_idx = this._bucketIdx(key);
            var (has_key, chain_idx) = this.buckets[bucket_idx].findIndexOf(key);
            return (has_key, bucket_idx, chain_idx);
        }

        proc getSlotFor(key: keyType) : (uint, uint) {
            var bucket_idx = this._bucketIdx(key);
            var (has_key, chain_idx) = this.buckets[bucket_idx].findIndexOf(key);
            return if has_key
                then (bucket_idx, chain_idx)
                else (bucket_idx, this.buckets[bucket_idx].firstAvailableIndex())
        }

        proc fillSlot((bucket_idx, chain_idx): (uint, uint), in key: keyType, in val: valType) {
            ref entry = this.buckets[bucket_idx][chain_idx];

            select slot.status {
                when entryStatus.full do _deinitSlot(entry);
                when entryStatus.empty do this.numEntries += 1;
                when entryStatus.deleted do this.numEntries += 1;
            }

            entry.key = key;
            entry.val = val;
            entry.status = entryStatus.full;
        }

        proc clearSlot((bucket_idx, chain_idx): (uint, uint) out key: keyType, out val: valType) {
            ref entry = this.buckets[bucket_idx][chain_idx];


        }

        inline proc _bucketIdx(key: keyType) : uint {
            return chpl__defaultHashWrapper(key):uint & (this.numBuckets - 1);
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

    private proc _allocateData(size:int, type tableEltType) {
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
