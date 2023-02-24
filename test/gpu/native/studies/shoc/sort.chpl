/*
sort.chpl is a  Chapel transliteration of SHOC sort
(the CUDA version of which is hosted at:
https://github.com/vetter/shoc/blob/master/src/cuda/level1/sort/Sort.cu)
*/

use IO;
use Time;
use List;
use CTypes;
use ResultDB;
use GPUDiagnostics;

config const noisy = false;
config const gpuDiags = false;
config const output = true;
config const perftest = false;
config const sz = 4;
config const passes = 5;
param SORT_BLOCK_SIZE = 128;
param SCAN_BLOCK_SIZE = 256;
param SORT_BITS = 32;
param WARP_SIZE = 32;

record innerArray {
  type t;
  var D = {0..-1};
  forwarding var A: [D] t;
}

on here.gpus[0] {

proc runSort(){
    // Number of key value pairs to sort scaled down by 2^20
    const problemSizes = [1, 8, 48, 96];
    var timer: stopwatch;

    var size = problemSizes[sz-1] : uint(32);
    // Scale back up by 2^20
    size = ((size * 1024 * 1024) / numBytes(uint(32))):uint(32);

    // Size of keys and vals in bytes
    const sizeInBytes : uint(32) = ( size * numBytes(uint(32)) ) : uint(32);
    // Create Input data on CPU (host)
    var hKeys : [0..<size] uint(32);
    var hVals : [0..<size] uint(32);

    // Space for Block Sums in scan kernel
    var numLevelsAllocated = 0;
    var maxNumScanElements = size;
    var numScanElts = maxNumScanElements;
    var level : uint (32)= 0;
    do{
        var numBlocks : uint(32) = max (1 , ceil (
                    numScanElts : real / (4 * SCAN_BLOCK_SIZE)) : int) : uint(32);
        if(numBlocks > 1) then level+=1;
        numScanElts = numBlocks;
    } while numScanElts > 1;

    /*var scanBlockSums : [0..#level+1] c_ptr(uint(32));*/
    var scanBlockSums : [0..#level+1] innerArray(uint(32));
    numLevelsAllocated = level + 1;
    numScanElts = maxNumScanElements;
    level = 0;
    do {
        var numBlocks : uint(32) = max (1 , ceil (
                    numScanElts : real / (4 * SCAN_BLOCK_SIZE)) : int) : uint(32);
        if(numBlocks > 1) {
            // Malloc device mem for block sums
            /*scanBlockSums[level] = c_malloc(uint(32), numBlocks);*/
            scanBlockSums[level].D = {0..#numBlocks};
            level+=1;
        }
        numScanElts = numBlocks;
    } while (numScanElts > 1);
    // Print the above vars to see if they match the expected values

    /*scanBlockSums[level] = c_malloc(uint(32), 1);*/
    scanBlockSums[level].D = {0..0};

    // Allcoate device mem for sorting kernels
    var dKeys, dVals, dTempKeys, dTempVals : [0..<size] uint(32);

    // Each thread in the sort kernel handles 4 elements

    const numSortGroups = size / (4 * SORT_BLOCK_SIZE);
    var dCounters, dCounterSums, dBlockOffsets : [0..<WARP_SIZE*numSortGroups] uint(32);

    var sortDb = new ResultDatabase("Sort Rate", "GB/s", atts="", attsSuffix=" items");
    var pciDb = new ResultDatabase("Sort Rate PCIe", "GB/s", atts="", attsSuffix=" items");
    var parityDb = new ResultDatabase("Sort Parity", "N", atts="", attsSuffix=" items");

    const iterations = passes;
    // They do timing using cudaEvent, ig we will just use our timer.
    for it in 0..#iterations:uint(32){
        // Initialize the host memory to some pattern
        for i in 0..<size:uint(32) {
            hVals[i] = (i%1024):uint(32);
            hKeys[i] = hVals[i];
        }

        // Copy inputs to GPU
        var transferTime = 0.0;
        // CudaEvent Record use replaced with timer
        timer.start();
        dKeys = hKeys;
        dVals = hVals;
        timer.stop();
        transferTime += timer.elapsed();
        timer.clear();

        // The CUDA version uses the uint4 type
        // It is a struct containing 4 uints.
        // In order to mimic that we calculate the appropriate
        // offsets into our array instead.
        // var outerArraySize = dKeys.size / 4; // Always a multiple of 4
        //                                 // So we don't need to round up
        // var dKeys4 = reshape(dKeys, {0..#outerArraySize, 0..3});
        // var dVals4 = reshape(dVals, {0..#outerArraySize, 0..3});
        // var dTempKeys4 = reshape(dTempKeys, {0..#outerArraySize, 0..3});
        // var dTempVals4 = reshape(dTempVals, {0..#outerArraySize, 0..3});

        // Perform Radix Sort 4 bits at a time
        timer.start();
        for i in 0..#SORT_BITS by 4{
            radixSortStep(4, i:uint(32), dKeys, dVals, dTempKeys, dTempVals,
            dCounters, dCounterSums, dBlockOffsets, scanBlockSums, size);
        }
        timer.stop();
        var kernelTime = timer.elapsed();
        timer.clear();

        // Readback Data from Device
        // dKeys = reshape(dKeys4, {0..#size});
        // dVals = reshape(dVals4, {0..#size});
        // dTempKeys = reshape(dTempKeys4, {0..#size});
        // dTempVals = reshape(dTempVals4, {0..#size});
        timer.start();
        hKeys = dKeys;
        hVals = dVals;
        timer.stop();
        transferTime += timer.elapsed();
        timer.clear();

        if(!verifySort(hKeys, hVals, size)){
            return;
        }

        // Print out results
        const gb = sizeInBytes * 2.0 / (1000.0 * 1000.0 * 1000.0);
        const sortRate = gb / kernelTime;
        const pciRate = gb / (kernelTime + transferTime);
        const parity = transferTime / kernelTime;
        if (noisy){
            writeln("Pass: ", it, "\nKernel Time: ", kernelTime, "\nTransfer Time: ", transferTime);
            writeln("Items: ", size);
            writeln("Sort Rate ", sortRate, " GB/s");
            writeln("Sort Rate PCIe ", pciRate, " GB/s");
            writeln("Sort Parity ",parity, " N\n");
        }
        sortDb.addToDatabase("%{#####}".format(size), sortRate);
        pciDb.addToDatabase("%{#####}".format(size), pciRate);
        parityDb.addToDatabase("%{#####}".format(size), parity);
    }
    if(output) {
        sortDb.printDatabaseStats();
        pciDb.printDatabaseStats();
        parityDb.printDatabaseStats();
    }
    if(perftest){
        sortDb.printPerfStats();
        pciDb.printPerfStats();
        parityDb.printPerfStats();
    }
}

if gpuDiags then startGPUDiagnostics();
runSort();
if gpuDiags {
  stopGPUDiagnostics();
  writeln(getGPUDiagnostics());
}


proc radixSortStep(nbits: uint(32), startbit: uint(32),
                    ref keys : [] uint(32), ref values : [] uint(32),
                    ref tempKeys : [] uint(32), ref tempValues : [] uint(32),
                    ref counters : [] uint(32), ref countersSum : [] uint(32),
                    ref blockOffsets : [] uint(32),
                    ref scanBlockSums : [] innerArray(uint(32)),
                    numElements: uint(32)){

    // Threads have either 4 or 2 elements each
    const radixGlobalWorkSize   = numElements / 4;
    const findGlobalWorkSize    = numElements / 2;
    const reorderGlobalWorkSize = numElements / 2;

    // Radix Kernel uses block size of 128, others use 256
    // (same as scan)
    const radixBlocks   = radixGlobalWorkSize   / SORT_BLOCK_SIZE;
    const findBlocks    = findGlobalWorkSize    / SCAN_BLOCK_SIZE;
    const reorderBlocks = reorderGlobalWorkSize / SCAN_BLOCK_SIZE;

    // radixSortBlocks Kernel here
    radixSortBlocks(radixGlobalWorkSize, nbits, startbit, tempKeys, tempValues, keys, values);

    // findRadixOffset Kernel here
    // Use offsets again
    findRadixOffsets(findGlobalWorkSize, tempKeys, counters, blockOffsets, startbit, numElements, findBlocks);

    scanArrayRecursive(countersSum, counters, 16*reorderBlocks, 0,
                       scanBlockSums);

    // reorderData Kernel here
    reorderData(reorderGlobalWorkSize, startbit, keys, values, tempKeys, tempValues,
         blockOffsets, countersSum, counters, reorderBlocks);
}

proc scanArrayRecursive(outArray : [] uint(32), inArray : [] uint(32),
                        numElements, level, ref blockSums){
    // Kernels hadle 8 elements per thread
    const numBlocks : uint(32) = max(1,ceil(numElements:real
        / (4.0:real * SCAN_BLOCK_SIZE)) : uint(32)) : uint(32);

    const sharedEltsPerBlock : uint(32) = (SCAN_BLOCK_SIZE * 2) : uint(32);
    const sharedMemSize : uint(32) = numBytes(uint(32)) * sharedEltsPerBlock;

    const fullBlock : bool = (numElements == numBlocks * 4 * SCAN_BLOCK_SIZE);

    // CUDA version uses dim3 for no apparent reason because they
    // created dim3 with only x populated for grid and block size

    //execute scan
    if(numBlocks>1){
        // Scan Kernel here
        ref t = blockSums[level].A;
        scanKernel(numBlocks, outArray, inArray, t, numElements, fullBlock, true);
    } else {
        // Different Scan Kernel here
        ref t = blockSums[level].A;
        scanKernel(numBlocks, outArray, inArray, t, numElements, fullBlock, false);
    }

    if(numBlocks > 1){
        ref t = blockSums[level].A;
        scanArrayRecursive(t, t, numBlocks, level+1, blockSums);
        // VectorAddUniform4 Kernel Here
        vectorAddUniform4(outArray, t, numElements);
    }
}

proc verifySort(ref keys : [] uint(32), ref values : [] uint(32), const size){
    var passed = true;
    for i in 0..<size-1 {
        if(keys[i] > keys[i+1]){
            passed = false;
            writeln("Fail at ", i, ": ", keys[i], " > ", keys[i+1]);
            break;
        }
    }
    write("Test ");
    if(passed) then writeln("Passed");
    else writeln("Failed");
    return passed;
}

// /////////////////////////////////////////////////////////////////////////////////
// // Kernels
// ////////////////////////////////////////////////////////////////////////////////

proc scanLSB(const val:uint(32), ref s_data : c_ptr(uint(32)), threadId: uint(32)){
    // Shared memory is 256 uints long, set first half to 0's
    var idx = threadId;
    s_data[idx] = 0 : uint(32);
    __primitive("gpu syncThreads");

    // Set second half to thread local sum i.e.
    // the sum of the 4 elems from global mem
    idx += SORT_BLOCK_SIZE : uint(32); // 128 in this case

    // Unrolled scan in local memory
    var t : uint(32);
    s_data[idx] = val;     __primitive("gpu syncThreads");
    t = s_data[idx -  1];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx -  2];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx -  4];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx -  8];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx - 16];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx - 32];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx - 64];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");

    return s_data[idx] - val;  // convert inclusive -> exclusive
}

proc scan4(idata : 4*uint(32), ref ptr : c_ptr(uint(32)), threadId : uint(32)){

    var val4: 4*uint(32) = idata;
    var sum: 4*uint(32);

    // Scan the 4 elements in idata within this thread
    sum[0] = val4[0];
    sum[1] = val4[1] + sum[0];
    sum[2] = val4[2] + sum[1];
    var val:uint(32) = val4[3] + sum[2];

    // Now scan those sums across the local work group
    val = scanLSB(val, ptr, threadId);

    val4[0] = val;
    val4[1] = val + sum[0];
    val4[2] = val + sum[1];
    val4[3] = val + sum[2];

    return val4;
}

proc radixSortBlocks(radixGlobalWorkSize, const nbits : uint(32), const startbit : uint(32),
                ref keysOut : [] uint(32), ref valuesOut : [] uint(32),
                ref keysIn : [] uint(32), ref valuesIn : [] uint(32)){

    foreach i in 0..<radixGlobalWorkSize:uint(32) {

        __primitive("gpu set blockSize", SORT_BLOCK_SIZE);

        var smVoidPtr : c_void_ptr = __primitive("gpu allocShared", 512*numBytes(uint(32)));
        var sMem = smVoidPtr : c_ptr(uint(32));

        // This does not work. Gives us an internal error
        // var tid = __primitive("gpu threadIdx x");
        const tid = i  % SORT_BLOCK_SIZE : uint(32);
        const localSize = SORT_BLOCK_SIZE: uint(32);


        // var key = [j in  keysIn.dim(1)] keysIn[i, j];
        // var value = valuesIn[i, 0..3];
        // var value =  [j in  keysIn.dim(1)] keysIn[i, j]; // Internal Error with looping over multidiemensional arrays

        // Load keys and vals from Global memory
        var key, value : 4*uint(32);
        const base = 4*i;
        for param j in 0..3:uint(32) do
            key[j] = keysIn[base + j];
        for param j in 0..3:uint(32) do
            value[j] = valuesIn[base + j];

        // For each of the 4 bits
        for shift in startbit:uint(32)..#nbits:uint(32) {
            // Check if LSB is 0
            var lsb : 4*uint(32); // Using tuple insted of uint4
            lsb[0] = !((key[0] >> shift) & 0x1);
            lsb[1] = !((key[1] >> shift) & 0x1);
            lsb[2] = !((key[2] >> shift) & 0x1);
            lsb[3] = !((key[3] >> shift) & 0x1);

            // Do an exclusive scan of how many elems have 0's in the LSB
            // When this is finished, address.n will contain the number of
            // elems with 0 in the LSB which precede elem n

            // We are going to inline the following function call due to multiple
            // issues with the compiler causing no kernels to be launched
            var address : 4*uint(32) = scan4(lsb, sMem, tid);

            var numTrueVoidPtr = __primitive("gpu allocShared", numBytes(uint(32)));
            var numtrue = numTrueVoidPtr : c_ptr(uint(32));

            // Store the total number of elems with an LSB of 0
            // to shared mem
            if(tid == localSize - 1){
                numtrue[0] = address[3] + lsb[3];
            }

            __primitive("gpu syncThreads");

            // Determine the rank -- position in the block
            // If you are 0 --> your position is the scan of 0's
            // If you are 1 --> your position is calculated below
            var rank: 4*uint(32);
            const idx : uint(32) = tid*4;
            rank[0] = if lsb[0] then address[0] else (numtrue[0] + idx     - address[0]);
            rank[1] = if lsb[1] then address[1] else (numtrue[0] + idx + 1 - address[1]);
            rank[2] = if lsb[2] then address[2] else (numtrue[0] + idx + 2 - address[2]);
            rank[3] = if lsb[3] then address[3] else (numtrue[0] + idx + 3 - address[3]);

            // Scatter keys into local mem
            sMem[(rank[0] & 3) * localSize + (rank[0] >> 2)] = key[0];
            sMem[(rank[1] & 3) * localSize + (rank[1] >> 2)] = key[1];
            sMem[(rank[2] & 3) * localSize + (rank[2] >> 2)] = key[2];
            sMem[(rank[3] & 3) * localSize + (rank[3] >> 2)] = key[3];

            __primitive("gpu syncThreads");

            // Read keys out of local mem into register, in prep for
            // write out to global mem
            key[0] = sMem[tid];
            key[1] = sMem[tid +     localSize];
            key[2] = sMem[tid + 2 * localSize];
            key[3] = sMem[tid + 3 * localSize];
            __primitive("gpu syncThreads");

            // Scatter values into local mem
            sMem[(rank[0] & 3) * localSize + (rank[0] >> 2)] = value[0];
            sMem[(rank[1] & 3) * localSize + (rank[1] >> 2)] = value[1];
            sMem[(rank[2] & 3) * localSize + (rank[2] >> 2)] = value[2];
            sMem[(rank[3] & 3) * localSize + (rank[3] >> 2)] = value[3];

            __primitive("gpu syncThreads");

            // Read keys out of local mem into register, in prep for
            // write out to global mem
            value[0] = sMem[tid];
            value[1] = sMem[tid + 1 * localSize];
            value[2] = sMem[tid + 2 * localSize];
            value[3] = sMem[tid + 3 * localSize];
            __primitive("gpu syncThreads");

        }
        for param j in 0..3:uint(32) do
            keysOut[base + j] = key[j];
        for param j in 0..3:uint(32) do
            valuesOut[base + j] = value[j];
    }
}

proc findRadixOffsets(findGlobalWorkSize, ref keys : [] uint(32), ref counters : [] uint(32),
        ref blockOffsets : [] uint(32), startbit :uint(32), numElements :uint(32), totalBlocks :uint(32) ){

    foreach i in 0..<findGlobalWorkSize:uint(32){

        __primitive("gpu set blockSize", SCAN_BLOCK_SIZE);

        var startVoidPtr = __primitive("gpu allocShared", 16*numBytes(uint(32)));
        var sStartPointers = startVoidPtr : c_ptr(uint(32));

        var sRadixVoidPtr = __primitive("gpu allocShared", 2*SCAN_BLOCK_SIZE*numBytes(uint(32)));
        var sRadix1 = sRadixVoidPtr : c_ptr(uint(32));

        const groupId = i / SCAN_BLOCK_SIZE : uint(32);
        const localId = i % SCAN_BLOCK_SIZE: uint(32);
        const groupSize = SCAN_BLOCK_SIZE : uint(32);

        var radix2 : 2*uint(32);
        radix2[0] = keys[2 * i];
        radix2[1] = keys[2 * i + 1];

        sRadix1[2 * localId]     = (radix2[0] >> startbit) & 0xF;
        sRadix1[2 * localId + 1] = (radix2[1] >> startbit) & 0xF;

        // Finds the position where the sRadix1 entries differ and stores start
        // index for each radix.
        if localId < 16 then sStartPointers[localId] = 0;

        __primitive("gpu syncThreads");

        if (localId > 0) && (sRadix1[localId] != sRadix1[localId - 1]) then
            sStartPointers[sRadix1[localId]] = localId;

        if sRadix1[localId + groupSize] != sRadix1[localId + groupSize - 1] then
            sStartPointers[sRadix1[localId + groupSize]] = localId + groupSize;

        __primitive("gpu syncThreads");

        if localId < 16 then
            blockOffsets[groupId*16 + localId] = sStartPointers[localId];

        __primitive("gpu syncThreads");

        // Compute the size of each block
        if (localId > 0) && (sRadix1[localId] != sRadix1[localId - 1]) then
            sStartPointers[sRadix1[localId - 1]] =
                localId - sStartPointers[sRadix1[localId - 1]];

        if sRadix1[localId + groupSize] != sRadix1[localId + groupSize - 1] then
          sStartPointers[sRadix1[localId + groupSize - 1]] =
               localId + groupSize - sStartPointers[sRadix1[localId +
                                                            groupSize - 1]];


        if localId == groupSize - 1 then
            sStartPointers[sRadix1[2 * groupSize - 1]] =
                2 * groupSize - sStartPointers[sRadix1[2 * groupSize - 1]];

        __primitive("gpu syncThreads");

        if localId < 16 then
            counters[localId * totalBlocks + groupId] = sStartPointers[localId];
    }

}

proc reorderData (reorderGlobalWorkSize, startbit: uint(32),
        ref outKeys : [] uint(32), ref outValues : [] uint(32),
        ref keys : [] uint(32), ref values : [] uint(32),
        ref blockOffsets : [] uint(32), ref offsets : [] uint(32),
        ref sizes : [] uint(32), totalBlocks : uint(32)) {

    foreach i in 0..<reorderGlobalWorkSize: uint(32){
        __primitive("gpu set blockSize", SCAN_BLOCK_SIZE);

        const GROUP_SIZE = SCAN_BLOCK_SIZE : uint(32);

        // sKeys2 in the CUDA version is an array of 256 uint2's (256*2 = 512)
        // Since we cannot allocate space for uint2 in Chapel we make a flat
        // array of size 512 and use the proper offsets to mimic the CUDA version
        var sKeys2VoidPtr = __primitive("gpu allocShared", 512*numBytes(uint(32)));
        var sKeys2 = sKeys2VoidPtr : c_ptr(uint(32));

        // Same deal with sValues2 as with sKeys2
        var sValues2VoidPtr = __primitive("gpu allocShared", 512*numBytes(uint(32)));
        var sValues2 = sValues2VoidPtr : c_ptr(uint(32));

        var sOffsetsVoidPtr = __primitive("gpu allocShared", 16*numBytes(uint(32)));
        var sOffsets = sOffsetsVoidPtr : c_ptr(uint(32));

        var sBlockOffsetsVoidPtr = __primitive("gpu allocShared", 16*numBytes(uint(32)));
        var sBlockOffsets = sBlockOffsetsVoidPtr : c_ptr(uint(32));

        // I do not port sKeys1 and sValues1 because of the way I handle sKeys2 and sValues2
        // sKeys1 and sValues1 are the flat versions of sKeys2 and sValues2, but since in
        // our implementation they are already flat we don't have to do anything.

        const blockId = i / SCAN_BLOCK_SIZE : uint(32);
        const threadId = i % SCAN_BLOCK_SIZE : uint(32);

        sKeys2  [threadId * 2]     = keys  [2 * i + 0];
        sKeys2  [threadId * 2 + 1] = keys  [2 * i + 1];

        sValues2[threadId * 2]     = values[2 * i + 0];
        sValues2[threadId * 2 + 1] = values[2 * i + 1];

        if(threadId < 16) {
            sOffsets[threadId] = offsets[threadId * totalBlocks + blockId];
            sBlockOffsets[threadId] = blockOffsets[blockId * 16 + threadId];
        }

        __primitive("gpu syncThreads");

        var radix : uint(32) = (sKeys2[threadId] >> startbit) & 0xF;
        var globalOffset : uint(32) = sOffsets[radix] + threadId - sBlockOffsets[radix];
        outKeys[globalOffset] = sKeys2[threadId];
        outValues[globalOffset] = sValues2[threadId];

        radix = (sKeys2[threadId + GROUP_SIZE] >> startbit) & 0xF;
        globalOffset = sOffsets[radix] + threadId + GROUP_SIZE
                    - sBlockOffsets[radix];

        outKeys[globalOffset] = sKeys2[threadId + GROUP_SIZE];
        outValues[globalOffset] = sValues2[threadId + GROUP_SIZE];
    }
}

proc scanLocalMem(const val : uint(32), ref s_data: c_ptr(uint(32)), in idx : uint(32)){

    // Shared mem is 512 uints long, set first half to 0
    s_data[idx] = 0 : uint(32);
    __primitive("gpu syncThreads");

    // Set 2nd half to thread local sum (sum of the 4 elems from global mem)
    idx += SCAN_BLOCK_SIZE : uint(32); // += 256

    var t : uint(32);
    s_data[idx] = val;     __primitive("gpu syncThreads");
    t = s_data[idx -  1];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx -  2];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx -  4];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx -  8];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx - 16];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx - 32];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx - 64];  __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");
    t = s_data[idx - 128]; __primitive("gpu syncThreads");
    s_data[idx] += t;      __primitive("gpu syncThreads");

    return s_data[idx-1];
}

proc scanKernel(numBlocks : uint(32), g_odata: [] uint(32),
                g_idata: [] uint(32), g_blockSums : [] uint(32),
                const n, const fullBlock: bool, const storeSum : bool){

    var globalSize : uint(32) = numBlocks * SCAN_BLOCK_SIZE;
    foreach gid in 0..<globalSize : uint(32) {

        __primitive("gpu set blockSize", SCAN_BLOCK_SIZE);


        var s_dataVoidPtr = __primitive("gpu allocShared", 512*numBytes(uint(32)));
        var s_data = s_dataVoidPtr : c_ptr(uint(32));

        //Load data insto shared mem
        var tempData : 4*uint(32);
        var threadScanT : 4*uint(32);
        var res: uint(32);
        // No reshape, index adjustment only
        // var inData = reshape(g_idata, {0..#g_idata.size/4, 0..3});

        const blockId = gid / SCAN_BLOCK_SIZE : uint(32);
        const tid = gid % SCAN_BLOCK_SIZE : uint(32);
        const i = gid*4 : uint(32);

        // If possible, read from global mem in a uint4 chunk
        if(fullBlock || (i+3) < n) {
            for j in 0..3:uint(32) {
                tempData[j] = g_idata[i + j];
            }
            threadScanT[0] = tempData[0];
            threadScanT[1] = tempData[1] + threadScanT[0];
            threadScanT[2] = tempData[2] + threadScanT[1];
            threadScanT[3] = tempData[3] + threadScanT[2];
            res = threadScanT[3];
        } else {
            // If not, read individual uints, scan and store in lmem
            threadScanT[0] = if i < n then g_idata[i] else 0:uint(32);
            threadScanT[1] = (if i+1 < n then g_idata[i+1] else 0:uint(32)) + threadScanT[0];
            threadScanT[2] = (if i+2 < n then g_idata[i+2] else 0:uint(32)) + threadScanT[1];
            threadScanT[3] = (if i+3 < n then g_idata[i+3] else 0:uint(32)) + threadScanT[2];
            res = threadScanT[3];
        }

        res = scanLocalMem(res, s_data, tid);
        __primitive("gpu syncThreads");

        // If we have to store the sum for the block, have the last work item
        // in the block write it out
        if storeSum && (tid == SCAN_BLOCK_SIZE - 1) then
            g_blockSums[blockId] = res + threadScanT[3];


        // write the results to global memory
        // ignore reshape just do index adjustment

        tempData[0] = res;
        tempData[1] = res + threadScanT[0];
        tempData[2] = res + threadScanT[1];
        tempData[3] = res + threadScanT[2];

        if fullBlock || (i + 3) < n {
            for j in 0..3: uint(32) {
                g_odata[gid * 4 + j] = tempData[j];
            }
        } else {
            if i < n { g_idata[i] = tempData[0];
            if (i+1) < n { g_idata[i+1] = tempData[1];
            if (i+2) < n { g_idata[i+2] = tempData[2]; } } }
        }
    }
}

proc vectorAddUniform4(d_vector: [] uint(32),
                       d_uniforms : [] uint(32), const n){

    // Math to get loop bounds:
    // CUDA Kernel Parameter: grid * thread
    // = numBlocks * SCAN_BLOCK_SIZE
    // = numElements / (4 * SCAN_BLOCK_SIZE) * SCAN_BLOCK_SIZE
    // = numElements / (4) = n/4
    foreach i in 0..<n/4 : uint(32) {

        __primitive("gpu set blockSize", SCAN_BLOCK_SIZE);

        const blockId = i / SCAN_BLOCK_SIZE : uint(32);
        const threadId = i % SCAN_BLOCK_SIZE : uint(32);

        var uniVoidPtr = __primitive("gpu allocShared", numBytes(uint(32)));
        var uni = uniVoidPtr : c_ptr(uint(32));

        if(threadId == 0) then uni[0] = d_uniforms[blockId];

        var address : uint(32) = threadId + (blockId * SCAN_BLOCK_SIZE * 4) : uint(32);

        __primitive("gpu syncThreads");

        // 4 elems per thread
        for idx in 0..3:uint(32) {
            if( address >= n) then break;
            d_vector[address] += uni[0];
            address += SCAN_BLOCK_SIZE : uint(32);
        }
    }
}
} // end on here.gpus[0]
