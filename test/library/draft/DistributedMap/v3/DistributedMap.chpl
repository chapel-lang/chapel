
module DistributedMap {
    private use Map;
    private use ChainTable;
    private use HaltWrappers;
    private use CyclicDist;
    private use IO;
    private use Math;
    private use Reflection;
    private use ChapelLocks;


    record distributedMap : writeSerializable {
        type keyType;
        type valType;

        var instance: shared distMapInternal(keyType, valType, ?)?;
        forwarding instance!;

        proc init(type keyType, type valType) {
            this.keyType = keyType;
            this.valType = valType;
            this.instance = new shared distMapInternal(keyType, valType);
        }

        proc init(type keyType, type valType, hasher) {
            this.keyType = keyType;
            this.valType = valType;
            this.instance = new shared distMapInternal(keyType, valType, hasher);
        }

        proc clear() {
            this.instance!.clear();
        }

        proc serialize(writer, ref serializer) throws {
            this.instance!.serialize(writer, serializer);
        }
    }

    class distMapInternal : writeSerializable {
        type keyType;
        type valType;

        @chpldoc.nodoc
        const targetLocales = Locales;

        @chpldoc.nodoc
        const locDom = {0..<targetLocales.size}
            dmapped new cyclicDist(startIdx=0, targetLocales=targetLocales);

        @chpldoc.nodoc
            var tables: [locDom] chainTable(keyType, valType);

        @chpldoc.nodoc
        var locks: [locDom] owned _LockWrapper =
            [i in locDom] new owned _LockWrapper();

        @chpldoc.nodoc
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

        proc extend(m: map(?)) {
            compilerError("unimplemented");
        }

        proc extend(other: distributedMap(?)) {
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

        override proc serialize(writer, ref serializer) throws {
            var fr = writer;
            for locIdx in this.locDom {
                on this.targetLocales[locIdx] {
                    fr.write("[", this.targetLocales[locIdx], ": ");
                    this.locks[locIdx].lock();
                    for entry in this.tables[locIdx].items() do
                        fr.write("{", entry.key, " : ", entry.val, "}, ");
                    this.locks[locIdx].unlock();
                    fr.write("], ");
                }
            }
        }

        // -------------------------------------------
        //  Reference Accessors
        // -------------------------------------------

        proc ref this(k: keyType) ref : valType
            where isDefaultInitializable(valType)
        {
            const loc = this._localeFor(k);
            var bucket_idx, chain_idx: uint(64);

            on loc {
                this.locks[loc.id].lock();
                const (found, foundSlot) = this.tables[loc.id].getFullSlotFor(k);
                if !found {
                    var v : this.valType;
                    const openSlot = this.tables[loc.id].getSlotFor(k);
                    this.tables[loc.id].fillSlot(openSlot, k, v);
                    bucket_idx = openSlot[0];
                    chain_idx = openSlot[1];
                } else {
                    bucket_idx = foundSlot[0];
                    chain_idx = foundSlot[1];
                }
                this.locks[loc.id].unlock();
            }
            ref ret = this.tables[loc.id].buckets[bucket_idx][chain_idx].val;
            return ret;
        }

        @chpldoc.nodoc
        proc const this(k: keyType) const : valType
            where shouldReturnRvalueByValue(valType) && !isNonNilableClass(valType)
        {
            const loc = this._localeFor(k);
            var ret : this.valType;
            on loc {
                this.locks[loc.id].lock();
                const (found, (bucket_idx, chain_idx)) = this.tables[loc.id].getFullSlotFor(k);
                if !found then boundsCheckHalt("map index " + k:string + " out of bounds");
                ret = this.tables[loc.id].buckets[bucket_idx][chain_idx].val;
                this.locks[loc.id].unlock();
            }
            return ret;
        }

        @chpldoc.nodoc
        proc const this(k: keyType) const ref : valType
            where !isNonNilableClass(valType)
        {
            const loc = this._localeFor(k);
            var bucket_idx, chain_idx: uint(64);
            on loc {
                this.locks[loc.id].lock();
                const (found, foundSlot) = this.tables[loc.id].getFullSlotFor(k);
                if !found then boundsCheckHalt("map index " + k:string + " out of bounds");
                bucket_idx = foundSlot[0];
                chain_idx = foundSlot[1];
                this.locks[loc.id].unlock();
            }
            ref ret = this.tables[loc.id].buckets[bucket_idx][chain_idx].val;
            return ret;
        }

        @chpldoc.nodoc
        proc const this(k: keyType)
            where isNonNilableClass(valType)
        {
            compilerError("Cannot access nilable class directly. Use an",
                            " appropriate accessor method instead.");
        }

        // -------------------------------------------
        //  Value Accessors
        // -------------------------------------------

        proc this(k : keyType) : valType throws {
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

        proc get(k: keyType, sentinel: valType) : valType {
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

        proc replace(k : keyType, in v: valType) : bool {
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

        proc addOrReplace(in k: keyType, in v: valType) {
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

        proc _incrementStaticCounts() {
            for t in this.tables do t.__incrementStaticCount();
        }

        proc _decrementStaticCounts() {
            for t in this.tables do t.__decrementStaticCount();
        }

        proc _maybeResizeAndBallance() {
            for idx in locDom {
                this.locks[idx].lock();
                this.tables[idx].__maybeResize();
                this.locks[idx].unlock();
            }

            // TODO: this might also be a good place to do a load ballance
            //         among locales (iff using default hasher)
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
            const h = (hash & (log2(this.numLocs) + 1)) % this.numLocs;
            // writeln("key: ", key, " -> ", hash, " : ", h);
            return h;
        }
    }

    record staticRefsManager : contextManager {
        type dmType;
        var dm: dmType;

        proc init(dm) {
            this.dmType = dm.type;
            this.dm = dm;
        }

        proc ref enterContext() ref: dmType {
            this.dm._incrementStaticCounts();
            return this.dm;
        }

        proc ref exitContext(in err: owned Error?) {
            this.dm._decrementStaticCounts();
            this.dm._maybeResizeAndBallance();
            if err then try! { throw err; }
        }
    }
}
