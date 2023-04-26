
module DistributedList {

    private use HaltWrappers;
    private use ChapelLocks;
    private use CyclicDist;
    private use BlockCycDist;

    private use IO;

    param DefaultBlockSize = 16,
          DefaultNumBufferedBlocksPerLocale = 2;

    /*

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
        param blockSize: int;

        pragma "no doc"
        const targetLocales = Locales;

        pragma "no doc"
        const locDom = {0..<targetLocales.size}
            dmapped Cyclic(startIdx=0, targetLocales=targetLocales);

        pragma "no doc"
        var locks: [locDom] ChapelLocks.chpl_LocalSpinlock =
            [i in locDom] new ChapelLocks.chpl_LocalSpinlock();

        pragma "no doc"
        var blockLists: [locDom] blockList(eltType, blockSize);

        pragma "no doc"
        var numEntries: atomic int;


        proc init(type eltType, param blockSize=DefaultBlockSize) {
            this.eltType = eltType;
            this.blockSize = blockSize;
            this.complete();
            this.numEntries.write(0);
        }

        proc init(arr: [?d] ?t, param blockSize=DefaultBlockSize)
            where d.rank == 1 && !d.stridable && d.dist._value.dsiIsLayout()
        {
            this.eltType = t;
            this.blockSize = blockSize;
            this.complete();

            const numBlocks = d.size / blockSize,
                  remainder = d.size % blockSize,
                  numBlocksPerLoc = numBlocks / this.locDom.size;

            // acquire memory to store the array
            for (loc, idx) in zip(this.targetLocales, this.locDom) do on loc do
                this.blockLists[idx].acquireBlocks(numBlocksPerLoc + DefaultNumBufferedBlocksPerLocale);

            // store full blocks on the appropriate locales
            for b in 0..#numBlocks {
                const locIdx = b % this.locDom.size;
                on this.targetLocales[locIdx] {
                    this.blockLists[locIdx].appendBlock(arr[(b*blockSize)..#blockSize]);
                }
            }

            // handle the final partial block if there is one
            if remainder > 0 {
                const rLocIdx = numBlocks % this.locDom.size,
                      lastFullBlockIdx = blockSize * numBlocks;

                on this.targetLocales[rLocIdx] {
                    for r in 0..<remainder {
                        const idx = lastFullBlockIdx + r,
                              (_, blockLayerIdx, eltIdx) = indicesFor(idx);

                        pragma "no auto destroy"
                        var cpy = arr[idx];

                        this.blockLists[rLocIdx].set(blockLayerIdx, eltIdx, cpy);
                    }
                    this.blockLists[rLocIdx].numFilled += remainder;
                }
            }

            this.numEntries.write(d.size);
        }

        // append a new element to the end of the list
        proc ref append(pragma "no auto destroy" in x: eltType): int {
            this.lockAll(); defer this.unlockAll();
            const nextIdx = this.numEntries.read(),
                  (locIdx, blockIdx, eltIdx) = indicesFor(nextIdx);

            // acquire memory if needed
            on this.targetLocales[locIdx] do
                if this.blockLists[locIdx].numBlocks <= blockIdx
                    then this.blockLists[locIdx].acquireBlocks();

            this.blockLists[locIdx].set(blockIdx, eltIdx, x);
            this.blockLists[locIdx].numFilled += 1;

            return this.numEntries.fetchAdd(1);
        }

        // append a group of elements to the end of the list
        proc ref append(x: [?d] eltType): range(d.idxType)
            where d.rank == 1 && !d.stridable && d.dist._value.dsiIsLayout()
        {
            this.lockAll(); defer this.unlockAll();

            const nextIdx = this.numEntries.read(),
                  newEltRange = nextIdx..#d.size,
                  newBlocksPerLoc = (d.size / this.blockSize) / this.locDom.size + 1,
                  (_, maxBlockIdx, _) = indicesFor(nextIdx),
                  (locRanges, blocksUsedPerLoc) =
                    rangesOnEachLocale(newEltRange, maxBlockIdx + newBlocksPerLoc);

            for locIdx in this.locDom do on this.targetLocales[locIdx] {
                // acquire memory if needed
                if this.blockLists[locIdx].numBlocks < blocksUsedPerLoc[locIdx] then
                    this.blockLists[locIdx].acquireBlocks();

                for ((globBlockStartIdx, globIdxRange), blockIdx) in zip(locRanges[locIdx], 0..) {
                    // handle full block
                    if globIdxRange.size == this.blockSize {
                        this.blockLists[locIdx].appendBlock(x[globIdxRange - nextIdx]);
                    }
                    // handle partial block
                    else {
                        for i in globIdxRange {
                            pragma "no auto destroy"
                            var cpy = x[i - nextIdx];

                            this.blockLists[locIdx].set(blockIdx, i - globBlockStartIdx, cpy);
                            this.blockLists[locIdx].numFilled += 1;
                        }
                    }
                }
            }

            this.numEntries.add(d.size);
            return newEltRange;
        }

        // does this list contain at least one 'x'?
        proc contains(x: eltType): bool {
            this.lockAll(); defer this.unlockAll();
            for e in this.these() do
                if e == x then return true;
            return false;
        }

        // return a reference to the list's first element
        proc first() ref {
            this.lockAll(); defer this.unlockAll();
            assert(this.numEntries.read() > 0, "list is empty; cannot get first element");
            const (locIdx, blockIdx, eltIdx) = indicesFor(0);
            return this.blockLists[locIdx].getRef(blockIdx, eltIdx);
        }

        // return a reference to the list's last element
        proc last() ref {
            this.lockAll(); defer this.unlockAll();
            assert(this.numEntries.read() > 0, "list is empty; cannot get last element");
            const (locIdx, blockIdx, eltIdx) = indicesFor(this.numEntries.read() - 1);
            return this.blockLists[locIdx].getRef(blockIdx, eltIdx);
        }

        // insert a new element at the given index. Shift all subsequent elements to the right
        proc ref insert(idx: int, pragma "no auto destroy" in x: eltType): bool {
            this.lockAll(); defer this.unlockAll();

            if boundsCheck(idx) {
                const nextIdx = this.numEntries.read();

                // acquire memory for the new element if needed
                const (nLoc, nBlock, nIdx) = indicesFor(nextIdx);
                on this.targetLocales[nLoc] {
                    if this.blockLists[nLoc].numBlocks <= nBlock
                        then this.blockLists[nLoc].acquireBlocks();

                    this.blockLists[nLoc].numFilled += 1;
                }

                // acquire memory to make room at the end of the list if needed
                const (nextLocIdx, nextBlockIdx, _) = indicesFor(nextIdx);
                on this.targetLocales[nextLocIdx] {
                    if this.blockLists[nextLocIdx].numBlocks < nextBlockIdx
                        then this.blockLists[nextLocIdx].acquireBlocks();
                }

                // shift elements to the right
                for i in (idx+1)..nextIdx by -1 {
                    const (dLoc, dBlock, dIdx) = indicesFor(i);
                    const (sLoc, sBlock, sIdx) = indicesFor(i-1);

                    on this.targetLocales[dLoc] {
                        ref src = this.blockLists[sLoc].getRef(sBlock, sIdx);
                        ref dst = this.blockLists[dLoc].getRef(dBlock, dIdx);
                        _move(src, dst);
                    }
                }

                // insert the new element
                const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
                on this.targetLocales[locIdx] {
                    this.blockLists[locIdx].set(blockIdx, eltIdx, x);
                }
                this.numEntries.add(1);

                return true;
            } else {
                _destroy(x);
                return false;
            }
        }

        // insert an array of elements. shift all subsequent elements 'd.size' slots to the right
        proc ref insert(idx: int, arr: [?d] eltType): bool
            where d.rank == 1 && !d.stridable && d.dist._value.dsiIsLayout()
        {
            this.lockAll(); defer this.unlockAll();
            if this.boundsCheck(idx) {

                const lastIdx = this.numEntries.fetchAdd(d.size);

                // shift all elements after 'idx' d.size indices to the right
                // TODO: aggregate communication instead of doing inter-locale shifts one element at a time
                // TODO: abstract out a separate method to shift elements (for use in both insert methods)
                for destIdx in idx+d.size..lastIdx+d.size by -1 {
                    const (dLoc, dBlock, dIdx) = indicesFor(destIdx);
                    const (sLoc, sBlock, sIdx) = indicesFor(destIdx - d.size);

                    on this.targetLocales[dLoc] {
                        // acquire memory if needed
                        if this.blockLists[dLoc].numBlocks <= dBlock
                            then this.blockLists[dLoc].acquireBlocks();

                        ref src = this.blockLists[sLoc].getRef(sBlock, sIdx);
                        ref dst = this.blockLists[dLoc].getRef(dBlock, dIdx);
                        _move(src, dst);

                        if destIdx > lastIdx
                            then this.blockLists[dLoc].numFilled += 1;
                    }
                }

                // insert the new elements starting at idx
                for (x, eIdx) in zip(arr, idx..) {
                    pragma "no auto destroy"
                    var cpy = x;

                    const (locIdx, blockIdx, eltIdx) = indicesFor(eIdx);
                    on this.targetLocales[locIdx] {
                        this.blockLists[locIdx].set(blockIdx, eltIdx, cpy);
                    }
                }

                return true;
            } else {
                return false;
            }
        }

        // remove up to 'count' occurrences of 'x' from the list
        //  return the number of elements that were removed
        proc ref remove(x: eltType, count: int = 1): int {
            this.lockAll(); defer this.unlockAll();
            var numRemoved = 0;
            const numToRemove = if count > 0 then count else max(int);
            for 0..#numToRemove {
                const idx = this._find(x);
                if idx > -1 {
                    // ('this.numEntries' is updated by '_pop()')
                    this._pop(idx);
                    numRemoved += 1;
                } else break;
            }
            return numRemoved;
        }

        // remove and return the last element in the list
        proc ref pop(): eltType {
            this.lockAll(); defer this.unlockAll();
            assert(this.numEntries.read() > 0, "list is empty; cannot pop last element");
            const lastIdx = this.numEntries.read() - 1,
                  (locIdx, blockIdx, eltIdx) = indicesFor(lastIdx);
            this.numEntries.sub(1);
            this.blockLists[locIdx].numFilled -= 1;

            return this.blockLists[locIdx].take(blockIdx, eltIdx);
        }

        // remove and return the element at a given index
        proc ref pop(idx: int): eltType {
            assert(boundsCheck(idx), "'idx' out of bounds in 'pop'");
            this.lockAll(); defer this.unlockAll();
            return this._pop(idx);
        }

        // non-locking pop
        pragma "no doc"
        proc _pop(idx: int): eltType {
            const lastIdx = this.numEntries.read() - 1;

            // take the value at idx
            const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
            const ret = this.blockLists[locIdx].take(blockIdx, eltIdx);

            // shift values to the left
            for i in idx..lastIdx {
                const (dLoc, dBlock, dIdx) = indicesFor(i);
                const (sLoc, sBlock, sIdx) = indicesFor(i+1);

                on this.targetLocales[dLoc] {
                    ref src = this.blockLists[sLoc].getRef(sBlock, sIdx);
                    ref dst = this.blockLists[dLoc].getRef(dBlock, dIdx);
                    _move(src, dst);
                }
            }

            const (lastLocIdx, lastBlockIdx, lastEltIdx) = indicesFor(lastIdx);
            _destroy(this.blockLists[lastLocIdx].getRef(lastBlockIdx, lastEltIdx));
            this.blockLists[lastLocIdx].numFilled -= 1;
            this.numEntries.sub(1);

            return ret;
        }

        // empty the list
        proc ref clear() {
            this.lockAll();
            for locIdx in this.locDom do
                on this.targetLocales[locIdx] do
                    this.blockLists[locIdx].clear();
            this.numEntries.write(0);
            this.unlockAll();
        }

        // return the number of times 'x' occurs in the list
        proc count(x: eltType): int {
            this.lockAll();
            var cnt = 0;
            coforall locIdx in this.locDom with (+ reduce cnt)
            // for locIdx in this.locDom
                do on this.targetLocales[locIdx] {
                    for val in this.blockLists[locIdx] {
                        if val == x then cnt += 1;
                    }
                }
            this.unlockAll();
            return cnt;
        }

        // return the first index of 'x' in the given range
        // return '-1' if 'x' is not found
        proc find(x: eltType, start: int = 0, end: int = -1): int {
            assert(boundsCheck(start), "'start' index out of bounds in 'find'");
            assert(end == -1 || boundsCheck(end), "'end' index out of bounds in 'find'");
            this.lockAll(); defer this.unlockAll();
            return this._find(x, start, end);
        }

        // non-locking find
        pragma "no doc"
        proc _find(x: eltType, start: int = 0, end: int = -1): int {
            const last = if end == -1 then (this.numEntries.read() - 1) else end,
                  (_, maxBlockIdx, _) = indicesFor(last),
                  (locRanges, _) = rangesOnEachLocale(start..last, maxBlockIdx);

            var minIdx = max(int);
            coforall locIdx in this.locDom with (min reduce minIdx)
            // for locIdx in this.locDom
                do on this.targetLocales[locIdx] {
                    forall ((globBlockStartIdx, globIdxRange), blockIdx) in zip(locRanges[locIdx], 0..) with (min reduce minIdx) {
                    // for ((globBlockStartIdx, globIdxRange), blockIdx) in zip(locRanges[locIdx], 0..) {
                        for i in globIdxRange do
                            if this.blockLists[locIdx].getRef(blockIdx, i - globBlockStartIdx) == x {
                                minIdx = min(minIdx, this.globalIndex(locIdx, blockIdx, i - globBlockStartIdx));
                                break;
                            }
                    }
            }

            return if minIdx < max(int) then minIdx else -1;
        }

        // get a copy of the value at 'idx'
        proc getValue(idx: int): eltType {
            this.lockAll(); defer this.unlockAll();
            assert(this.boundsCheck(idx));
            const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
            return this.blockLists[locIdx].blocks[blockIdx][eltIdx];
        }

        // set the value at 'idx'
        proc ref set(idx: int, pragma "no auto destroy" in x: eltType): bool {
            this.lockAll(); defer this.unlockAll();

            if !this.boundsCheck(idx) {
                _destroy(x);
                return false;
            }

            const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
            this.blockLists[locIdx].set(blockIdx, eltIdx, x);
            return true;
        }

        // apply an updater to the value at the given index
        proc ref update(idx: int, updater) throws {
            import Reflection;

            this.lockAll(); defer this.unlockAll();
            assert(this.boundsCheck(idx));

            const (locIdx, blockIdx, eltIdx) = this.indicesFor(idx);
            // on this.targetLocales[locIdx] {
                ref slot = this.blockLists[locIdx].getRef(blockIdx, eltIdx);

                if !Reflection.canResolveMethod(updater, "this", idx, slot) then
                    compilerError('`list.update()` failed to resolve method ' +
                                updater.type:string + '.this() for arguments (' +
                                idx.type:string + ', ' + slot.type:string + ')');
                return updater(idx, slot);
            // }
        }

        // return a reference to the element at 'idx'
        proc ref this(idx: int) ref {
            assert(this.boundsCheck(idx), "invalid list index: " + idx:string);
            const (locIdx, blockIdx, eltIdx) = indicesFor(idx);
            return this.blockLists[locIdx].getRef(blockIdx, eltIdx);
        }

        proc const isEmpty(): bool {
            return this.numEntries.read() == 0;
        }

        inline proc const size {
            return this.numEntries.read();
        }

        inline proc const indices {
            return 0..<this.numEntries.read();
        }

        // create a block-cyclic distributed array from the distributed list
        proc toArray(): [] eltType {
            this.lockAll();
            const dom = {0..<this.numEntries.read()} dmapped
                BlockCyclic(startIdx=0, blocksize=this.blockSize, targetLocales=this.targetLocales);

            var a : [dom] this.eltType;
            coforall (loc, locIdx) in zip(this.targetLocales, this.locDom) do on loc {
                for (elt, blockIdx, eltIdx) in this.blockLists[locIdx].valsAndIndices() do
                    a[this.globalIndex(locIdx, blockIdx, eltIdx)] = elt;
            }
            this.unlockAll();

            return a;
        }

        // TODO: make a parallel iterator
        iter these() {
            for i in 0..<this.numEntries.read() {
                const (locIdx, blockIdx, eltIdx) = indicesFor(i);
                yield this.blockLists[locIdx].getRef(blockIdx, eltIdx);
            }
        }

        proc writeThis(fr) throws {
            fr.write('[');
            for (elt, i) in zip(this.these(), 0..) {
                if i > 0 then fr.write(", ");
                fr.write(elt);
            }
            fr.write(']');
        }

        // TODO: readThis()?

        // ---------------------
        // Helper methods
        // ---------------------

        pragma "no doc"
        inline proc deinit() {
            for (loc, locIdx) in zip(this.targetLocales, this.locDom) do on loc {
                this.blockLists[locIdx]._empty();
            }
            this.numEntries.write(0);
        }

        pragma "no doc"
        // convert a global list index to:
        //   (locale_idx, block_idx, entry_idx_in_block)
        inline proc indicesFor(idx: int): (int, int, int) {
            return (
                idx / this.blockSize % this.locDom.size,
                idx / (this.blockSize * this.locDom.size),
                idx % this.blockSize
            );
        }

        pragma "no doc"
        inline proc globalIndex(locIdx: int, blockIdx: int, eltIdx: int): int {
            return blockIdx * this.blockSize * this.locDom.size + // full rows
                   locIdx * this.blockSize + // full blocks on this row
                   eltIdx; // elements in this block
        }

        pragma "no doc"
        inline proc lockAll() {
            for lock in this.locks do lock.lock();
        }

        pragma "no doc"
        inline proc unlockAll() {
            for lock in this.locks do lock.unlock();
        }

        // TODO: toggle this with global bounds checking flag
        pragma "no doc"
        inline proc boundsCheck(idx): bool {
            return idx >= 0 && idx < this.numEntries.read();
        }

        pragma "no doc"
        /* split a set of global indices across the corresponding blocks on each locale

            returns:
            (an array of arrays, where each locale's array contains:
                an entry for each of it's blocks with:
                    (
                        starting global index,
                        a range of global indices intersecting with the provided indices
                    )
             an array with the number of blocks on each locale that intersected with the provided range
            )
        */
        proc rangesOnEachLocale(r: range(idxType=int), numBlockLayers: int):
            ([this.locDom][0..numBlockLayers] (int, range(idxType=int)), [this.locDom] int)
        {
            var startsAndRanges: [this.locDom][0..numBlockLayers] (int, range(idxType=int)),
                numUsedPerLocale: [this.locDom] int;

            for locIdx in this.locDom {
                for locBlockIdx in 0..numBlockLayers {
                    const blockStartingIdx = globalIndex(locIdx, locBlockIdx, 0),
                          blockRange = blockStartingIdx..#blockSize;

                    const globalIdxRange =
                        if r.contains(blockRange) then blockRange
                        else if r.contains(blockRange.first) then blockRange.first..r.last
                        else if r.contains(blockRange.last) then r.first..blockRange.last
                        else if blockRange.contains(r) then r
                        else 0..<0;

                    if globalIdxRange.size > 0 then
                        numUsedPerLocale[locIdx] += 1;

                    startsAndRanges[locIdx][locBlockIdx] = (blockStartingIdx, globalIdxRange);
                }
            }

            return (startsAndRanges, numUsedPerLocale);
        }

        pragma "no doc"
        inline proc _destroy(ref item: eltType) {
            chpl__autoDestroy(item);
        }
    }

    // a list of blocks stored on a single locale
    // as elements are added, the number of blocks can be expanded to accommodate them

    //  within this data structure:
    //  - 'block_idx' refers to the zero-based index of a block
    //  - 'elt_idx' refers to the zero-based index of an element within a single block
    record blockList {
        type eltType;
        param blockSize : int;  // the number of elements per block. Defaults to 16

        var blocks: _ddata(_ddata(eltType)) = nil;

        var numBlocks: int;     // the total number of blocks currently allocated
        var numFilled: int;     // the total number of occupied elements in this blockList
                                //  (the caller is expected to update 'numFilled' manually
                                //   when modifying the blockList except when using the
                                //   'appendBlock' method)

        proc init(type eltType, param blockSize: int) {
            this.eltType = eltType;
            this.blockSize = blockSize;
            this.numBlocks = DefaultNumBufferedBlocksPerLocale;
            this.numFilled = 0;

            this.complete();
            on this {
                this.blocks = this._makeBlockArray(this.numBlocks);
                for i in 0..<this.numBlocks do this.blocks[i] = this._makeBlock();
            }
        }

        // replace the current blocks with n new blocks
        //  n defaults to twice the current number of blocks
        proc acquireBlocks(n: int = this.numBlocks * 2) {
            on this {
                // allocate new blocks
                assert(n > this.numBlocks);
                var blocks_new = this._makeBlockArray(n);

                // transfer the old data into the new blocks
                for b in 0..#this.numBlocks do
                    if this.blocks[b] == nil
                        then blocks_new[b] = this._makeBlock();
                        else blocks_new[b] = this.blocks[b];

                // allocate the remaining space
                for b in this.numBlocks..<n {
                    assert(blocks_new[b] == nil);
                    blocks_new[b] = this._makeBlock();
                    assert(blocks_new[b] != nil);
                }

                // replace old arrays
                this._freeBlockArray(this.blocks, this.numBlocks);
                this.blocks = nil;
                this.blocks = blocks_new;
                this.numBlocks = n;
            }

        }

        // how many full blocks are allocated but not occupied?
        inline proc numOpenBlocks(): int {
            return this.numBlocks - (this.numFilled / this.blockSize);
        }

        // set the value of an element with a particular block_idx and elt_idx to a given value
        proc set(block_idx: int, elt_idx: int, pragma "no auto destroy" in x: eltType) {
            assert(block_idx < this.numBlocks);
            ref slot = this.blocks[block_idx][elt_idx];
            _destroy(slot);
            _move(x, slot);
        }

        // remove and return the value at a given block_idx and elt_idx
        proc take(block_idx: int, elt_idx: int) {
            use MemMove;
            return moveFrom(this.blocks[block_idx][elt_idx]);
        }

        // return a reference to an element at a particular block_idx and elt_idx
        proc getRef(block_idx: int, elt_idx: int) ref {
            assert(this.blocks[block_idx] != nil);
            ref ret = this.blocks[block_idx][elt_idx];
            return ret;
        }

        // assumes that the current block is full
        // and that the next block has already been allocated
        proc appendBlock(arr: [?d] eltType) {
            const nextBlockIdx = this.numFilled / this.blockSize;

            assert(d.size == this.blockSize);
            assert(nextBlockIdx < this.numBlocks);
            assert(this.blocks[nextBlockIdx] != nil);

            for (x, i) in zip(arr, 0..) {
                pragma "no auto destroy"
                var cpy = x;
                this.set(nextBlockIdx, i, cpy);
            }
            this.numFilled += d.size;
        }

        // free this blockList's memory and reallocate the default # of blocks
        proc clear() {
            // free all memory
            this._empty();

            // reallocate the default number of blocks
            this.blocks = this._makeBlockArray(DefaultNumBufferedBlocksPerLocale);
            for i in 0..<DefaultNumBufferedBlocksPerLocale do this.blocks[i] = this._makeBlock();

            // set meta parameters
            this.numFilled = 0;
            this.numBlocks = DefaultNumBufferedBlocksPerLocale;
        }

        proc _empty() {
            // free each block
            for b in 0..#this.numBlocks {
                if this.blocks[b] == nil { continue; }
                else {
                    for i in 0..<this.blockSize {
                        ref x = this.blocks[b][i];
                        _destroy(x);
                    }
                    _ddata_free(this.blocks[b], this.blockSize);
                    this.blocks[b] = nil;
                }
            }

            // free blocks array
            this._freeBlockArray(this.blocks, this.numBlocks);
            this.blocks = nil;
        }

        // yield all values along with their corresponding block_idx and elt_idx
        iter valsAndIndices() {
            const numFullBlocks = this.numFilled / this.blockSize;

            // yield the first n full blocks
            for i in 0..<numFullBlocks {
                for j in 0..<this.blockSize {
                    yield (this.blocks[i][j], i, j);
                }
            }

            // yield the partial last block
            for j in 0..<(this.numFilled % this.blockSize) {
                yield (this.blocks[numFullBlocks][j], numFullBlocks, j);
            }
        }

        // yield all values
        iter these() ref {
            const numFullBlocks = this.numFilled / this.blockSize;

            // yield the first n full blocks
            for i in 0..<numFullBlocks {
                for j in 0..<this.blockSize {
                    yield this.getRef(i, j);
                }
            }

            // yield the partial last block
            for j in 0..<(this.numFilled % this.blockSize) {
                yield this.getRef(numFullBlocks, j);
            }
        }

        proc _freeBlockArray(data: _ddata(_ddata(eltType)), size: int) {
            _ddata_free(data, size);
        }

        proc _makeBlockArray(size: int) {
            return _ddata_allocate(_ddata(eltType), size);
        }

        proc _makeBlock() {
            var callPostAlloc = false;
            var ret = _ddata_allocate_noinit(eltType, this.blockSize, callPostAlloc);
            if callPostAlloc then _ddata_allocate_postalloc(ret, this.blockSize);
            return ret;
        }

        pragma "no doc"
        inline proc _destroy(ref item: eltType) {
            chpl__autoDestroy(item);
        }
    }

    pragma "no doc"
    pragma "unsafe"
    inline proc _move(ref src: ?t, ref dst: t) lifetime src == dst {
      __primitive("=", dst, src);
    }

}
