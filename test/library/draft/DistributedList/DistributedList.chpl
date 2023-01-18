
module DistributedList {

    private use HaltWrappers;
    private use ChapelLocks;
    private use CyclicDist;
    private use BlockCycDist;

    const DefaultBlockSize = 16,
          DefaultNumBlocksPerLocale = 2,

    /*

    Questions:
    - how should bounds checking be handled?
        Should it be turned on/off with a configurable parameter like the serial list?
        Should methods throw or halt when an index is out of bounds?

    - How should locking/concurrency work?
        do operations that have the potential to shift values between locales need
        to lock the entire list?


    Block layout:

    |     locale 0     |     locale 1     |      ...         |     locale N     |
    -----------------------------------------------------------------------------
    | block 0 [ ... ]  | block 0 [ ... ]  | block 0 [ ... ]  | block 0 [ ... ]  |
    | block 1 [ ... ]  | block 1 [ ... ]  | block 1 [ ... ]  | block 1 [ ... ]  |
    | ...              | ...              | ...              | ...              |
    | block n [ ... ]  | block n [ ... ]  | block n [ ... ]  |


    */

    record distributedList {
        type eltType;

        var blockSize: int;

        pragma "no doc"
        const targetLocales = Locales;

        pragma "no doc"
        const locDom = {0..<targetLocales.size}
            dmapped Cyclic(startIdx=0, targetLocales=targetLocales);

        pragma "no doc"
        var locks: [locDom] owned ChapelLocks.chpl_LocalSpinlock; =
            [i in locDom] new owned ChapelLocks.chpl_LocalSpinlock();

        pragma "no doc"
        var blockLists: [locDom] blockList(eltType);

        pragma "no doc"
        var size atomic int;

        proc init(type eltType, blockSize=DefaultBlockSize) {
            this.eltType = eltType;
            this.blockSize = blockSize;
            this.size = 0;
        }

        proc ref append(in x: eltType): int {
            this.lockAll();
            var lastIdx = this.size.read() - 1;
            const (locIdx, blockIdx, eltIdx) = indicesFor(lastIdx);

            this.blockLists[locIdx].set(blockIdx, eltIdx, x);
            this.blockLists[locIdx].numFilled += 1;
            this.unlockAll();

            return this.size.fetchAdd(1);
        }

        proc contains(x: eltType): bool {
            this.lockAll(); defer this.unlockAll();
            for e in this do
                if e == x then return true;
            return false;
        }

        proc first(): ref {
            const (locIdx, blockIdx, eltIdx) = indicesFor(0);
            this.locks[locIdx].lock(); defer this.locks[lockIdx].unlock();
            return this.blockLists[locIdx].getRef(blockIdx, eltIdx);
        }

        proc last(): ref {
            const (locIdx, blockIdx, eltIdx) = indicesFor(this.size.read() - 1);
            this.locks[locIdx].lock(); defer this.locks[lockIdx].unlock();
            return this.blockLists[locIdx].getRef(blockIdx, eltIdx);
        }

        proc ref insert(idx: int, in x: eltType): bool {
            const lastIdx = this.size.read() - 1;
            if boundsCheck(idx) {
                this.lockAll();

                // acquire memory for the new element if needed
                const (nLoc, nBlock, nIdx) = indicesFor(lastIdx);
                on this.targetLocales[nLoc] do
                    if this.blockLists[nLoc].numBlocks < nBlock
                        then this.blockLists[nLoc].acquireBlocks();

                // shift elements to the right
                for i in (idx+1)..lastIdx by -1 {
                    const (dLoc, dBlock, dIdx) = indicesFor(i);
                    const (sLoc, sBlock, sIdx) = indicesFor(i-1);

                    on this.targetLocales[dLoc] {
                        this.blockLists[dLoc].set(dBlock, dIdx,
                            this.blockLists[sLoc].take(sBlock, sIdx)
                        );
                    }
                }

                // insert the new element
                const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
                on this.targetLocales[locIdx] {
                    this.blockLists[locIdx].set(blockIdx, eltIdx, x);
                    this.blockLists[locIdx].numFilled += 1;
                }
                this.size.add(1);

                this.unlockAll();
                return true;
            } else {
                return false;
            }
        }

        proc ref insert(idx: int, arr: [?d] eltType): bool {
            if this.boundsCheck(idx) {
                this.lockAll();

                // shift elements to the right
                // TODO: aggregate the communication instead of doing inter-locale shifts one element at a time
                for i in (idx+d.size)..this.size.read() by -1 {
                    const (dLoc, dBlock, dIdx) = indicesFor(i);
                    const (sLoc, sBlock, sIdx) = indicesFor(i-d.size);

                    on this.targetLocales[dLoc] {
                        // acquire memory if needed
                        if this.blockLists[dLoc].numBlocks < dBlock
                            then this.blockLists[dLoc].acquireBlocks();

                        this.blockLists[dLoc].set(dBlock, dIdx,
                            this.blockLists[sLoc].take(sBlock, sIdx)
                        );
                    }
                }

                // insert the new elements
                for (e, eIdx) in zip(arr, idx..) {
                    const (locIdx, blockIdx, eltIdx) = indicesFor(eIdx);
                    on this.targetLocales[locIdx] {
                        this.blockLists[locIdx].set(blockIdx, eltIdx, e);
                        this.blockLists[locIdx].numFilled += 1;
                    }
                }
                this.size.add(d.size);

                this.unlockAll();
                return true;
            } else {
                return false;
            }
        }

        proc ref remove(x: eltType, count: int = 1): int {
            this.lockAll(); defer this.unlockAll();
            for i in 0..<(if count > 0 then count else max(int)) {
                const idx = this.find(x);
                if idx > -1
                    then this.pop(idx);
                    else return i - 1;
            }
            return count;
        }

        proc ref pop(): eltType {
            this.lockAll(); defer this.unlockAll();
            const lastIdx = this.size.read() - 1;
            const (locIdx, blockIdx, eltIdx) = indicesFor(lastIdx);
            this.size.sub(1);
            this.blockLists[locIdx].numFilled -= 1;
            return this.blockLists[locIdx].take(blockIdx, eltIdx);
        }

        proc ref pop(idx: int): eltType {
            assert(boundsCheck(idx), "'idx' out of bounds in 'pop'");

            this.lockAll();
            const lastIdx = this.size.read() - 1;

            // take the value at idx
            const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
            const ret = this.blockLists[locIdx].take(blockIdx, eltIdx);

            // shift values to the left
            for i in (idx + 1)..lastIdx {
                const (dLoc, dBlock, dIdx) = indicesFor(i+1);
                const (sLoc, sBlock, sIdx) = indicesFor(i);

                on this.targetLocales[dLoc] {
                    this.blockLists[dLoc].set(dBlock, dIdx,
                        this.blockLists[sLoc].take(sBlock, sIdx)
                    );
                }
            }

            const (locIdx, _, _) = indicesFor(lastIdx);
            this.blockLists[locIdx].numFilled -= 1;
            this.unlockAll();

            this.size.sub(1);
            return ret;
        }

        proc ref clear() {
            this.lockAll();
            for locIdx in this.locDom do
                on this.targetLocales[locIdx] do
                    this.blockLists[locIdx].clear();
            this.size.write(0);
            this.unlockAll();
        }

        proc find(x: eltType, start: int = 0, end: int = -1) {
            this.lockAll(); defer this.unlockAll();
            for i in start..<(if end == -1 then this.size.read() else end) {
                const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
                on this.targetLocales[locIdx] do
                    if this.blockLists[locIdx].getRef(blockIdx, eltIdx) == x then
                        return idx;
            }
            return -1;
        }

        proc getValue(idx: int): eltType {
            const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
            return this.blockLists[blockIdx].blocks[block_idx][elt_idx];
        }

        proc ref set(idx: int, in x: eltType): bool {
            use memMove;
            if idx >= this.size.read() then return false;

            const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
            moveInitialize(this.blockLists[locIdx].blocks[blockIdx][eltIdx], x);
            return true;
        }

        proc ref this(idx: int) ref {
            const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
            return this.blockLists[locIdx].getRef(blockIdx, eltIdx);
        }

        proc const isEmpty(): bool {
            return this.size.read() == 0;
        }

        inline proc const size {
            return this.size.read();
        }

        inline proc const indices {
            return 0..<this.size.read();
        }

        proc const toArray(): [] eltType {
            const dom = {0..<this.size.read()} dmapped
                BlockCyclic(startIdx=0, blockSize=this.blockSize, targetLocales=this.targetLocales);

            var a : [dom] this.eltType;

            this.lockAll();

            // Do we know that BlockCyclic will align indices to locales in the same fashion we have?
            for (loc, locIdx) in zip(this.targetLocales, this.locDom) do on loc {
                for (elt, idx) in this.blockLists[locIdx] do
                    a[idx] = elt;
            }
            this.unlockAll();

            return a;
        }

        iter these() {
            for i in 0..<this.size.read() {
                const (locIdx, blockIdx, eltIdx) = indicesFor(i);
                yield this.blockLists[locIdx].getRef(blockIdx, eltIdx);
            }
        }

        // ---------------------
        // Helper methods
        // ---------------------

        pragma "no doc"
        // (locale_idx, block_idx, entry_idx_in_block)
        inline proc indicesFor(idx: int): (int, int, int) {
            return (
                idx / this.blockSize % this.locDom.size,
                idx / (this.blockSize * this.locDom.size),
                idx % this.blockSize
            );
        }

        pragma "no doc"
        inline proc lockAll() {
            for lock in this.locks do lock.lock();
        }

        pragma "no doc"
        inline proc unlockAll() {
            for lock in this.locks do lock.unlock();
        }

        pragma "no doc"
        inline proc boundsCheck(idx): bool {
            return idx >= 0 && idx < this.size.read();
        }

    }

    // a list of blocks stored on a single locale
    // as elements are added, the number of blocks can be expanded to accommodate them
    record blockList {
        type eltType;
        var blockSize : int;

        var blocks: _ddata(_ddata(eltType)) = nil;

        var capacity: int;
        var numBlocks;
        var numFilled;

        proc init(type eltType, blockSize: int) {
            this.eltType = eltType;
            this.blockSize = blockSize;
            this.capacity = DefaultNumBlocksPerLocale * blockSize;
            this.numBlocks = DefaultNumBlocksPerLocale;
            this.numFilled = 0;

            this.complete();
            this.blocks = _ddata_allocate(_ddata(this.eltType), DefaultNumBlocksPerLocale);
            for i in 0..<DefaultNumBlocksPerLocale do this.blocks[i] = makeBlock();
        }

        proc makeBlock() {
            var callPostAlloc = false;
            var ret = _ddata_allocate_noinit(eltType, this.blockSize, callPostAlloc);
            if callPostAlloc then _ddata_allocate_postalloc(ret, this.blockSize);
            return ret;
        }

        proc acquireBlocks(n: int = this.numBlocks * 2) {
            // allocate new blocks
            assert(n >= this.numBlocks);
            var blocks_new = _ddata_allocate(_ddata(this.eltType, n));

            // transfer the old data into the new blocks
            for b in 0..<this.numBlocks do
                blocks_new[b] = this.blocks[b];

            // allocate the remaining space
            for b in this.numBlocks..<n do
                blocks_new[b] = makeBlock();

            // free old blocks
            _ddata_free(this.blocks, this.capacity);

            // set meta parameters
            this.blocks = blocks_new;
            this.capacity = n * this.blockSize;
            this.numBlocks = n;
        }

        proc set(block_idx: int, elt_idx: int, in x: eltType) {
            assert(block_idx < this.numBlocks);
            this.blocks[block_idx][elt_idx] = x;
        }

        proc take(block_idx: int, elt_idx: int) {
            use MemMove;
            return moveFrom(this.blocks[block_idx][elt_idx]);
        }

        proc getRef(block_idx: int, elt_idx: int) ref {
            ref ret = this.blocks[block_idx][elt_idx];
            return ret;
        }

        proc clear() {
            // free blocks
            __ddata_free(this.blocks, this.capacity);

            // reallocate the default number of blocks
            this.blocks = _ddata_allocate(_ddata(this.eltType), DefaultNumBlocksPerLocale);
            for i in 0..<DefaultNumBlocksPerLocale do this.blocks[i] = makeBlock();

            // set meta parameters
            this.numFilled = 0;
            this.numBlocks = DefaultNumBlocksPerLocale;
            this.capacity = DefaultNumBlocksPerLocale * this.blockSize;
        }

        iter these() {
            for i in 0..<(this.numBlocks-1) {
                for j in 0..<this.blockSize {
                    yield this.blocks[i][j];
                }
            }
            for j in 0..<(this.numFilled % this.blockSize) {
                yield this.blocks[this.numBlocks-1][j];
            }
        }
    }

}
