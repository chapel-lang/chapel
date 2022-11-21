
module DistributedMap {
    private use Map;
    private use ChainTable;
    private use HaltWrappers;
    private use CyclicDist;
    private use IO;
    private use Reflection;
    private use ChapelLocks;

    record distributedMap {
        type keyType;
        type valType;

        pragma "no doc"
        const targetLocales = Locales;

        pragma "no doc"
        const locDom = {0..<targetLocales.size}
            dmapped Cyclic(startIdx=0, targetLocales=targetLocales);

        pragma "no doc"
            var tables: [locDom] chainTable(keyType, valType);

        pragma "no doc"
        var locks: [locDom] owned _LockWrapper =
            [i in locDom] new owned _LockWrapper();

        pragma "no doc"
        var localeHasher;

        // -------------------------------------------
        //  Initializers
        // -------------------------------------------

        proc init(type keyType, type valType) {
            this.keyType = keyType;
            this.valType = valType;
            this.localeHasher = new defaultLocaleHasher(keyType, Locales.size);
        }

        proc init(type keyType, type valType, hasher)
            // where canResolveMethod(hasher, "this")
        {
            this.keyType = keyType;
            this.valType = valType;
            this.localeHasher = hasher;
        }

        // -------------------------------------------
        //  Whole map operations
        // -------------------------------------------

        proc clear() {
            for l in this.locks do l.lock();
            for t in this.tables do t.clear();
            for l in this.locks do l.unlock();
        }

        proc size {
            for l in this.locks do l.lock();
            var res = + reduce this.tables.numEntries;
            for l in this.locks do l.unlock();
            return res;
        }

        proc fuzzySize() {
            return + reduce this.tables.numEntries;
        }

        inline proc isEmpty() {
            return this.size == 0;
        }

        proc extend(m: map) {
            compilerError("unimplemented");
        }

        proc extend(other: distributedMap) {
            compilerError("unimplemented");
        }

        proc toArray(): [] (keyType, valType) {
            compilerError("unimplemented");
        }

        proc keysToArray(): [] keyType {
            compilerError("unimplemented");
        }

        proc valuesToArray(): [] valType {
            compilerError("unimplemented");
        }

        // -------------------------------------------
        //  Reference Accessors
        // -------------------------------------------

        proc getReference(k: keyType) ref {
            const loc = this._localeFor(k);
            var ret;
            on loc {
                this.locks[loc.id].lock();
                const (found, (bucket_idx, chain_idx)) = this.tables[loc.id].getFullSlotFor(k);
                if !found then boundsCheckHalt("map index " + k:string + " out of bounds");
                ref valref = this.tables[loc.id].buckets[bucket_idx][chain_idx].val;
                ret = valref;
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        proc ref this(k: keyType) ref : valType
            where isDefaultInitializable(valType)
        {
            const loc = this._localeFor(k);
            var ret: valType;
            on loc {
                this.locks[loc.id].lock();
                const (found, (bucket_idx, chain_idx)) = this.tables[loc.id].getFullSlotFor(k);
                if !found {
                    var v : this.keyType;
                    const openSlot = this.tables[loc.id].getSlotFor(k);
                    this.tables[loc.id].fillSlot(openSlot, k, v);
                    ret = this.tables[loc.id].buckets[openSlot[0]][openSlot[1]].val;
                } else {
                    ret = this.tables[loc.id].buckets[bucket_idx][chain_idx].val;
                }
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        pragma "no doc"
        proc const this(k: keyType) const : valType
            where shouldReturnRvalueByValue(valType) && !isNonNilableClass(valType)
        {
            const loc = this._localeFor(k);
            var ret : valType;
            on loc {
                this.locks[loc.id].lock();
                const (found, (bucket_idx, chain_idx)) = this.tables[loc.id].getFullSlotFor(k);
                if !found then boundsCheckHalt("map index " + k:string + " out of bounds");
                ret = this.tables[loc.id].buckets[bucket_idx][chain_idx].val;
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        pragma "no doc"
        proc const this(k: keyType) const ref : valType
            where !isNonNilableClass(valType)
        {
            const loc = this._localeFor(k);
            var ret: valType;
            on loc {
                this.locks[loc.id].lock();
                const (found, (bucket_idx, chain_idx)) = this.tables[loc.id].getFullSlotFor(k);
                if !found then boundsCheckHalt("map index " + k:string + " out of bounds");
                ret = this.tables[loc.id].buckets[bucket_idx][chain_idx].val;
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        pragma "no doc"
        proc const this(k: keyType)
            where isNonNilableClass(valType)
        {
            compilerError("Cannot access nilable class directly. Use an",
                            " appropriate accessor method instead.");
        }

        // -------------------------------------------
        //  Value Accessors
        // -------------------------------------------

        proc getValue(k : keyType) : valType throws {
            const loc = this._localeFor(k);
            var ret : valType;
            on loc {
                this.locks[loc.id].lock();
                const (found, (bucket_idx, chain_idx)) = this.tables[loc.id].getFullSlotFor(k);
                if !found
                    then ret = this.tables[loc.id].buckets[bucket_idx][chain_idx].val : this.valType;
                    else throw new KeyNotFoundError(k: string);
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        proc getValue(k: keyType, sentinel: valType) : valType {
            const loc = this._localeFor(k);
            var ret : valType;
            on loc {
                this.locks[loc.id].lock();
                const (found, (bucket_idx, chain_idx)) = this.tables[loc.id].getFullSlotFor(k);
                if !found
                    then ret = this.tables[loc.id].buckets[bucket_idx][chain_idx].val : this.valType;
                    else ret = sentinel;
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        proc getAndRemove(k: keyType) : valType {
            const loc = this._localeFor(k);
            var ret : valType;
            on loc {
                this.locks[loc.id].lock();
                const (found, slot) = this.tables[loc.id].getFullSlotFor(k);
                if !found
                    then ret = this.tables[loc.id].remove(slot, k, ret);
                    else throw new KeyNotFoundError(k: string);
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        proc contains(k: keyType): bool {
            const loc = this._localeFor(k);
            var ret = false;
            on loc {
                this.locks[loc.id].lock();
                const (found, _) = this.tables[loc.id].getFullSlotFor(k);
                ret = found;
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        // -------------------------------------------
        //  Modifiers
        // -------------------------------------------

        proc add(in k: keyType, in v: valType): bool {
            const loc = this._localeFor(k);
            var ret = true;
            on loc {
                this.locks[loc.id].lock();
                const (found, _) = this.tables[loc.id].getFullSlotFor(k);
                if !found {
                    const openSlot = this.tables[loc.id].getSlotFor(k);
                    this.tables[loc.id].fillSlot(openSlot, k, v);
                } else {
                    ret = false;
                }
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        proc set(k : keyType, in v: valType) : bool {
            const loc = this._localeFor(k);
            var ret = true;
            on loc {
                this.locks[loc.id].lock();
                const (found, slot) = this.tables[loc.id].getFullSlotFor(k);
                if found
                    then this.tables[loc.id].fillSlot(slot, k, v);
                    else ret = false;
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        proc addOrSet(in k: keyType, in v: valType) {
            const loc = this._localeFor(k);
            on loc {
                this.locks[loc.id].lock();
                const slot = this.tables[loc.id].getSlotFor(k);
                this.tables[loc.id].fillSlot(slot, k, v);
                this.locks[loc.id].unlock();
            }
        }

        proc remove(k: keyType): bool {
            const loc = this._localeFor(k);
            var ret = true;
            on loc {
                this.locks[loc.id].lock();
                const (found, slot) = this.tables[loc.id].getFullSlotFor(k);
                var kout, vout;
                if found
                    then this.tables[loc.id].remove(slot, kout, vout);
                    else ret = false;
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        // -------------------------------------------
        //  Utilities
        // -------------------------------------------

        proc _localeFor(key: keyType): locale {
            const lidx = this.localeHasher(key);
            if !this.locDom.contains(lidx) then halt("Invalid locale hash!");
            return this.targetLocales[lidx];
        }
    }

    record defaultLocaleHasher {
        type keyType;
        var numLocs: int;
        proc init(type kt, nl) {
            this.keyType = kt;
            this.numLocs = nl;
        }
        proc this(key: keyType): int {
            const hash = chpl__defaultHashWrapper(key);
            return hash % this.numLocs;
        }
    }

    record staticRefsManager {
        type dmType;
        var dm: dmType;

        proc init(dm) {
            this.dmType = dm.type;
            this.dm = dm;
        }

        proc ref enterThis() ref: dmType {
            for t in this.dm.tables do t.__incrementStaticCount();
            return this.dm;
        }

        proc ref leaveThis(in err: owned Error?) {
            for t in this.dm.tables {
                t.__decrementStaticCount();
                t.__maybeResize();
            }
            if err then try! { throw err; }
        }
    }
}
