
module DistributedList {

    private use HaltWrappers;
    private use ChapelLocks;
    private use CyclicDist;

    const DefaultBlockSize = 16,
          DefaultNumBlocksPerLocale = 2,

    /*


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
        var locks: [locDom] owned _LockWrapper =
            [i in locDom] new owned _LockWrapper();

        pragma "no doc"
        var locListBlocks: [locDom] blockList(eltType);

        pragma "no doc"
        var lastIdx atomic int;

        proc init(type eltType, blockSize=DefaultBlockSize) {
            this.eltType = eltType;
            this.blockSize = blockSize;
            this.lastIdx = 0;
        }

        // TODO: make this safe for concurrent use
        proc ref append(in x: eltType): int {
            var lastIdxVal = this.lastIdx.read();
            const (locIdx, blockIdx, eltIdx) = indicesFor(lastIdxVal);

            this.locks[locIdx].lock();
                this.locListBlocks[locIdx].set(blockIdx, eltIdx, x);
                this.locListBlocks[locIdx].numFilled += 1;
            this.locks[locIdx].unlock();

            return this.lastIdx.fetchAdd(1);
        }

        proc contains(x: eltType): bool {

        }

        proc first(): ref {

        }

        proc last(): ref {

        }

        proc ref insert(idx: int, in x: eltType): bool {
            const lastIdxVal = this.lastIdx.read();
            if idx < lastIdxVal {
                for l in this.locks do l.lock();

                // acquire memory for the new element if needed
                const (nLoc, nBlock, nIdx) = indicesFor(lastIdxVal+1);
                on this.targetLocales[nLoc] do
                    if this.locListBlocks[nLoc].numBlocks < nBlock
                        then this.locListBlocks[nLoc].acquireBlocks();

                // shift elements to the right
                // TODO: aggregate the communication instead of doing inter-locale shifts one element at a time
                for i in (idx+1)..this.lastIdx.read() by -1 {
                    const (dLoc, dBlock, dIdx) = indicesFor(i);
                    const (sLoc, sBlock, sIdx) = indicesFor(i-1);

                    on this.targetLocales[dLoc] {
                        this.locListBlocks[dLoc].set(dBlock, dIdx,
                            this.locListBlocks[sLoc].take(sBlock, sIdx)
                        );
                    }
                }

                // insert the new element
                const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
                on this.targetLocales[locIdx] {
                    this.locListBlocks[locIdx].set(blockIdx, eltIdx, x);
                    this.locListBlocks[locIdx].numFilled += 1;
                }


                for l in this.locks do l.unlock();
                return true;
            } else {
                return false;
            }
        }

        proc ref insert(idx: int, arr: [?d] eltType): bool {
            if idx < this.lastIdx.read() {
                for l in this.locks do l.lock();

                // shift elements to the right
                // TODO: aggregate the communication instead of doing inter-locale shifts one element at a time
                for i in (idx+d.size)..this.lastIdx.read() by -1 {
                    const (dLoc, dBlock, dIdx) = indicesFor(i);
                    const (sLoc, sBlock, sIdx) = indicesFor(i-d.size);

                    on this.targetLocales[dLoc] {
                        // acquire memory if needed
                        if this.locListBlocks[dLoc].numBlocks < dBlock
                            then this.locListBlocks[dLoc].acquireBlocks();

                        this.locListBlocks[dLoc].set(dBlock, dIdx,
                            this.locListBlocks[sLoc].take(sBlock, sIdx)
                        );
                    }
                }

                // insert the new elements
                for (e, eIdx) in zip(arr, idx..) {
                    const (locIdx, blockIdx, eltIdx) = indicesFor(eIdx);
                    on this.targetLocales[locIdx] {
                        this.locListBlocks[locIdx].set(blockIdx, eltIdx, e);
                        this.locListBlocks[locIdx].numFilled += 1;
                    }
                }

                for l in this.locks do l.unlock();
                return true;
            } else {
                return false;
            }
        }

        proc ref remove(x: eltType, count: int = 1): int {

        }

        proc ref pop(): eltType {

        }

        proc ref clear() {

        }

        proc find(x: eltType, start: int = 0, end: int = -1) {

        }

        proc getValue(idx: int): eltType {

        }

        proc ref set(idx: int, in x: eltType): bool {

        }

        proc ref this(idx: int) ref {

        }

        proc size {

        }

        proc indices {

        }

        proc toArray(): [] eltType {
            const dom = {0..<this.lastIdx.read()} dmapped
                Cyclic(startIdx=0, targetLocales=this.targetLocales);

            var a : [dom] this.eltType;

            for (loc, locIdx) in zip(this.locks, this.locDom) do on loc {
                this.locks[locIdx].lock();

                for (elt, idx) in this.locListBlocks[locIdx] do
                    a[idx] = elt;

                this.locks[locIdx].unlock();
            }

            return a;
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
            assert(n >= this.numBlocks);
            var blocks_new = _ddata_allocate(_ddata(this.eltType, n));

            for b in 0..<this.numBlocks do
                blocks_new[b] = this.blocks[b];

            for b in this.numBlocks..<n do
                blocks_new[b] = makeBlock();

            _ddata_free(_arrays, this.capacity);
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
