use Sort;
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

config var m = 10;
config var n = 10;

/* Pulled from Arkouda, used within radix sort for mergin arrays */
config param RSLSD_bitsPerDigit = 16;
private param bitsPerDigit = RSLSD_bitsPerDigit;
private param numBuckets = 1 << bitsPerDigit; // these need to be const for comms/performance reasons
private param maskDigit = numBuckets-1; // these need to be const for comms/performance reasons

/* Pulled from Arkouda, gets the maximum bit width of the array and if there are any negative numbers */
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

/* Pulled from Arkouda, for two arrays returns array with bit width and negative information */
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

/* Pulled from Arkouda, get the digits for the current rshift. Signbit needs to be inverted for
   negative values */
inline proc getDigit(key: int, rshift: int, last: bool, negs: bool): int {
    const invertSignBit = last && negs;
    const xor = (invertSignBit:uint << (RSLSD_bitsPerDigit-1));
    const keyu = key:uint;
    return (((keyu >> rshift) & (maskDigit:uint)) ^ xor):int;
}

var src = blockDist.createArray({0..<m}, int);
var dst = blockDist.createArray({0..<m}, int);

fillRandom(src, 0, n-1);
fillRandom(dst, 0, n-1);

writeln("src = ", src);
writeln("dst = ", dst);
writeln();

var (totalDigits, bitWidths, negs) = getNumDigitsNumericArrays(src, dst);
writeln("totalDigits = ", totalDigits);
writeln("bitWidths = ", bitWidths);
writeln("negs = ", negs);
writeln();

/* Pulled from Arkouda to merge two arrays into one for radix sort */
proc mergeNumericArrays(param numDigits, size, totalDigits, bitWidths, negs) throws {
    var merged = blockDist.createArray({0..<size}, numDigits*uint(bitsPerDigit));
    var curDigit = numDigits - totalDigits;

    var nBits = bitWidths[0];
    var neg = negs[0];
    const r = 0..#nBits by bitsPerDigit;
    for rshift in r {
        const myDigit = (nBits-1 - rshift) / bitsPerDigit;
        const last = myDigit == 0;
        forall (m, a) in zip(merged, src) {
            m[curDigit+myDigit] =  getDigit(a, rshift, last, neg):uint(bitsPerDigit);
        }
    }
    curDigit += r.size;
    nBits = bitWidths[1];
    neg = negs[1];
    for rshift in r {
        const myDigit = (nBits-1 - rshift) / bitsPerDigit;
        const last = myDigit == 0;
        forall (m, a) in zip(merged, dst) {
            m[curDigit+myDigit] =  getDigit(a, rshift, last, neg):uint(bitsPerDigit);
        }
    }
    curDigit += r.size;
    return merged;
}

/* Pulled from Arkouda, runs the merge function and then the sort function */
proc mergedArgsort(param numDigits) {
    var merged = mergeNumericArrays(numDigits, m, totalDigits, bitWidths, negs);
    writeln("unsorted merged = ", merged);

    var AI = blockDist.createArray(merged.domain, (merged.type, int));
    var iv = blockDist.createArray(merged.domain, int);

    AI = [(a, i) in zip(merged, merged.domain)] (a, i);
    Sort.TwoArrayDistributedRadixSort.twoArrayDistributedRadixSort(AI, comparator=myDefaultComparator);
    iv = [(a, i) in AI] i;
    writeln("  sorted merged = ", AI);
    writeln("             iv = ", iv);

    return iv;
}

if totalDigits <=  4 { mergedArgsort( 4); }
else if totalDigits <=  8 { mergedArgsort( 8); }
else if totalDigits <= 16 { mergedArgsort(16); }


