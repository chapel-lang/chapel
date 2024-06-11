use Sort;
use Search;
use BlockDist;
use Random;
use BitOps;
use Reflection;

import Reflection.canResolveMethod;
record ContrivedComparator {
    const dc = new DefaultComparator();
    proc keyPart(a, i: int) {
        if canResolveMethod(dc, "keyPart", a, 0) {
            return dc.keyPart(a, i);
        } else if isTuple(a) {
            return tupleKeyPart(a, i);
        } else {
            compilerError("No keyPart method for eltType ", a.type:string);
        }
    }
    proc tupleKeyPart(x: _tuple, i:int) {
        proc makePart(y): uint(64) {
            var part: uint(64);
            // get the part, ignore the section
            const p = dc.keyPart(y, 0)(1);
            // assuming result of keyPart is int or uint <= 64 bits
            part = p:uint(64); 
            // If the number is signed, invert the top bit, so that
            // the negative numbers sort below the positive numbers
            if isInt(p) {
                const one:uint(64) = 1;
                part = part ^ (one << 63);
            }
            return part;
        }
        var part: uint(64);
        if isTuple(x[0]) && (x.size == 2) {
            for param j in 0..<x[0].size {
                if i == j {
                    part = makePart(x[0][j]);
                }
            }
            if i == x[0].size {
                part = makePart(x[1]);
            }
            if i > x[0].size {
                return (-1, 0:uint(64));
            } else {
                return (0, part);
            }
        } else {
            for param j in 0..<x.size {
                if i == j {
                    part = makePart(x[j]);
                }
            }
            if i >= x.size {
                return (-1, 0:uint(64));
                } else {
                return (0, part);
            }
        }
    }
}
const myDefaultComparator = new ContrivedComparator();

config var m = 50;
config var n = 10;

// Pulled from Arkouda, used within radix sort for merging arrays
config param RSLSD_bitsPerDigit = 16;
private param bitsPerDigit = RSLSD_bitsPerDigit;

// these need to be const for comms/performance reasons
private param numBuckets = 1 << bitsPerDigit;
private param maskDigit = numBuckets-1;

/* Pulled from Arkouda, gets the maximum bit width of the array and if there are
   any negative numbers */
inline proc getBitWidth(a: [?aD] int): (int, bool) {
    var aMin = min reduce a;
    var aMax = max reduce a;
    var wPos = if aMax >= 0 then numBits(int) - clz(aMax) else 0;
    var wNeg = if aMin < 0 then numBits(int) - clz((-aMin)-1) else 0;
    const signBit = if aMin < 0 then 1 else 0;
    const bitWidth = max(wPos, wNeg) + signBit;
    const negs = aMin < 0;

    return (bitWidth, negs);
}

/* Pulled from Arkouda, for two arrays returns array with bit width and negative 
   information */
proc getNumDigitsNumericArrays(arr1, arr2) throws {
    var bitWidths: [0..<2] int;
    var negs: [0..<2] bool;
    var totalDigits: int;

    (bitWidths[0], negs[0]) = getBitWidth(arr1);
    totalDigits += (bitWidths[0] + (bitsPerDigit-1)) / bitsPerDigit;

    (bitWidths[1], negs[1]) = getBitWidth(arr2);
    totalDigits += (bitWidths[1] + (bitsPerDigit-1)) / bitsPerDigit;

    return (totalDigits, bitWidths, negs);
}

/* Pulled from Arkouda, get the digits for the current rshift. Signbit needs to 
   be inverted for negative values */
inline proc getDigit(key: int, rshift: int, last: bool, negs: bool): int {
    const invertSignBit = last && negs;
    const xor = (invertSignBit:uint << (RSLSD_bitsPerDigit-1));
    const keyu = key:uint;

    return (((keyu >> rshift) & (maskDigit:uint)) ^ xor):int;
}

/* Pulled from Arkouda, return an array of all values from array a 
   whose index corresponds to a true value in array truth */
proc boolIndexer(a: [?aD] ?t, truth: [aD] bool) throws {
    var iv: [truth.domain] int = (+ scan truth);
    var pop = iv[iv.size-1];
    var ret = blockDist.createArray({0..<pop}, t);

    forall (i, eai) in zip(a.domain, a) do 
        if (truth[i]) then ret[iv[i]-1] = eai; // Needs aggregator.
    
    return ret;
}

var srcOriginal = blockDist.createArray({0..<m}, int);
var dstOriginal = blockDist.createArray({0..<m}, int);

fillRandom(srcOriginal, 0, n-1);
fillRandom(dstOriginal, 0, n-1);

// 0. Symmetrize the edge lists. Usually, done only for undirected graphs.
var src = blockDist.createArray({0..<2*m}, int);
var dst = blockDist.createArray({0..<2*m}, int);
src[0..<m] = srcOriginal; src[m..<2*m] = dstOriginal;
dst[0..<m] = dstOriginal; dst[m..<2*m] = srcOriginal;
m = 2*m;

writeln("Original edge list: ");
writeln("srcOriginal = ", srcOriginal);
writeln("dstOriginal = ", dstOriginal);
writeln();

writeln("Symmetrized edge list: ");
writeln("src = ", src);
writeln("dst = ", dst);
writeln();

var (totalDigits, bitWidths, negs) = getNumDigitsNumericArrays(src, dst);
writeln("Diagnostic information for merging arrays before radix sort:");
writeln("totalDigits = ", totalDigits);
writeln("bitWidths = ", bitWidths);
writeln("negs = ", negs);
writeln();

/* Pulled from Arkouda to merge two arrays into one for radix sort */
proc mergeNumericArrays(param numDigits, size, totalDigits, bitWidths, negs) {
    var merged = blockDist.createArray(
        {0..<size}, 
        numDigits*uint(bitsPerDigit)
    );
    var curDigit = numDigits - totalDigits;

    var nBits = bitWidths[0];
    var neg = negs[0];
    const r = 0..#nBits by bitsPerDigit;
    for rshift in r {
        const myDigit = (nBits-1 - rshift) / bitsPerDigit;
        const last = myDigit == 0;
        forall (m, a) in zip(merged, src) {
            m[curDigit+myDigit]= getDigit(a,rshift,last,neg):uint(bitsPerDigit);
        }
    }
    curDigit += r.size;
    nBits = bitWidths[1];
    neg = negs[1];
    for rshift in r {
        const myDigit = (nBits-1 - rshift) / bitsPerDigit;
        const last = myDigit == 0;
        forall (m, a) in zip(merged, dst) {
            m[curDigit+myDigit]= getDigit(a,rshift,last,neg):uint(bitsPerDigit);
        }
    }
    curDigit += r.size;

    return merged;
}

/* Pulled from Arkouda, runs the merge function and then the sort function */
proc mergedArgsort(param numDigits) {
    var merged = mergeNumericArrays(numDigits, m, totalDigits, bitWidths, negs);

    var AI = blockDist.createArray(merged.domain, (merged.eltType, int));
    var iv = blockDist.createArray(merged.domain, int);
    // writeln("unsorted merged = ", AI);

    AI = [(a, i) in zip(merged, merged.domain)] (a, i);
    Sort.TwoArrayDistributedRadixSort.twoArrayDistributedRadixSort(
        AI, comparator=myDefaultComparator
    );
    iv = [(_, i) in AI] i;

    return iv;
}

/* Pulled from Arkouda, removes duplicate keys from sorted arrays */
proc uniqueFromSorted(sorted: [?aD] ?eltType, param needCounts = true) {
    var truth = blockDist.createArray(aD, bool);
    truth[0] = true;
    [(t,s,i) in zip(truth,sorted,aD)] if i > aD.low { t = (sorted[i-1] != s); }
    var allUnique: int = + reduce truth;

    if allUnique == aD.size then return sorted;

    var iv: [truth.domain] int = (+ scan truth);
    var pop = iv[iv.size - 1];

    var segs = blockDist.createArray({0..<pop}, int);
    var ukeys = blockDist.createArray({0..<pop}, eltType);

    forall i in truth.domain {
        if truth[i] == true {
            var idx = i; 
            segs[iv[i]-1] = idx; // Needs aggregator.
        }
    }

    // Needs aggregator.
    forall (_, uk, seg) in zip(segs.domain, ukeys, segs) do uk = sorted[seg];

    return ukeys;
}

var iv = blockDist.createArray({0..<m}, int);
if totalDigits <=  4 { iv = mergedArgsort( 4); }
else if totalDigits <=  8 { iv = mergedArgsort( 8); }
else if totalDigits <= 16 { iv = mergedArgsort(16); }

/***********************************************/
/**********EDGE LIST PROCESSING STEPS***********/
/***********************************************/
// 1. Sort the edge list.
var sortedSrc = src[iv];
var sortedDst = dst[iv];
writeln("Edge lists after sorting:");
writeln("sortedSrc = ", sortedSrc);
writeln("sortedDst = ", sortedDst);
writeln();

// 2. Remove self-loops.
var loops = sortedSrc != sortedDst;
var sortedSrcNoLoops = boolIndexer(sortedSrc, loops);
var sortedDstNoLoops = boolIndexer(sortedDst, loops);
writeln("Edge lists after removing self-loops:");
writeln("sortedSrcNoLoops = ", sortedSrcNoLoops);
writeln("sortedDstNoLoops = ", sortedDstNoLoops);
writeln();

// 3. Remove duplicates.
var edgesAsTuples = blockDist.createArray(
    sortedSrcNoLoops.domain, (int,int)
);

forall (e, i) in zip(edgesAsTuples, edgesAsTuples.domain) do 
    e = (sortedSrcNoLoops[i], sortedDstNoLoops[i]);

var uniqueEdges = uniqueFromSorted(edgesAsTuples);
var eD = uniqueEdges.domain;
var sortedSrcNoLoopsNoDups = blockDist.createArray(eD,int);
var sortedDstNoLoopsNoDups = blockDist.createArray(eD,int);

forall (e,u,v) in zip(uniqueEdges,sortedSrcNoLoopsNoDups,sortedDstNoLoopsNoDups) {
    u = e[0];
    v = e[1];
}

writeln("Edge lists after removing duplicates:");
writeln("sortedSrcNoLoopsNoDups = ", sortedSrcNoLoopsNoDups);
writeln("sortedDstNoLoopsNoDups = ", sortedDstNoLoopsNoDups);
writeln();

// 4. Create one-up indices of vertices starting from 0 and ending with n-1.
var allVertices = blockDist.createArray({0..<eD.size*2}, int);
allVertices[0..<eD.size] = sortedSrcNoLoopsNoDups;
allVertices[eD.size..<eD.size*2] = sortedDstNoLoopsNoDups;
Sort.TwoArrayDistributedRadixSort.twoArrayDistributedRadixSort(allVertices);
var uniqueVertices = uniqueFromSorted(allVertices);
var sortedSrcNoLoopsNoDupsOneUp = blockDist.createArray(eD, int);
var sortedDstNoLoopsNoDupsOneUp = blockDist.createArray(eD, int);

forall i in eD {
    sortedSrcNoLoopsNoDupsOneUp[i] = Search.binarySearch(
        uniqueVertices,
        sortedSrcNoLoopsNoDups[i]
    )[1];
    sortedDstNoLoopsNoDupsOneUp[i] = Search.binarySearch(
        uniqueVertices,
        sortedDstNoLoopsNoDups[i]
    )[1];
}

writeln("Edge lists after creating one-up indices:");
writeln("uniqueVertices = ", uniqueVertices);
writeln("sortedSrcNoLoopsNoDupsOneUp = ", sortedSrcNoLoopsNoDupsOneUp);
writeln("sortedDstNoLoopsNoDupsOneUp = ", sortedDstNoLoopsNoDupsOneUp);
writeln();

/*************************************************/
/**********DATA STRUCTURE CREATION STEPS**********/
/*************************************************/

/**********EDGE-CENTRIC DATA STRUCTURE**********/

// 1. Get neighbor count.
var neighborCount = blockDist.createArray(uniqueVertices.domain, atomic int);
forall u in sortedSrcNoLoopsNoDupsOneUp do neighborCount[u].add(1);
writeln(neighborCount);
var neighborCountNotAtomic = blockDist.createArray(uniqueVertices.domain, int);
forall i in neighborCount.domain do 
    neighborCountNotAtomic[i] = neighborCount[i].read();

// 2. Run prefix sum to get segments array.
var neighborCountScan = + scan neighborCountNotAtomic;
var segments = blockDist.createArray({0..uniqueVertices.size}, int);
segments[0] = 0;
segments[1..] = neighborCountScan;
writeln(segments);

writeln("Edge-centric data structure:");
writeln("src = ", sortedSrcNoLoopsNoDupsOneUp);
writeln("dst = ", sortedDstNoLoopsNoDupsOneUp);
writeln("seg = ", segments);
writeln();

/**********VERTEX-CENTRIC DATA STRUCTURE**********/

