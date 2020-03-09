// Tests block-matrix multiply of the form needed in LU-decomposition. 
// The following diagram illustrates this process:
//
//     +-----+-----+-----+-----+
//     |     |bbbbb|bbbbb|bbbbb|  Solve for the dotted region by
//     |     |bbbbb|bbbbb|bbbbb|  multiplying the 'a' and 'b' region.
//     |     |bbbbb|bbbbb|bbbbb|  The 'a' region is a block column, the
//     +-----+-----+-----+-----+  'b' region is a block row.
//     |aaaaa|.....|.....|.....|
//     |aaaaa|.....|.....|.....|
//     |aaaaa|.....|.....|.....|
//     +-----+-----+-----+-----+
//     |aaaaa|.....|.....|.....|
//     |aaaaa|.....|.....|.....|
//     |aaaaa|.....|.....|.....|
//     +-----+-----+-----+-----+
//
// Assume there's a 1 to 1 mapping of blocks to locales. In this case for a
// locale in the dotted region to perform the necessary matrix multiply it will
// need a copy of the block in the 'a' region in the same block-row and a copy
// of the block in the 'b' region in the same block-column.

use Math;

config const n = 10;
const localesAcross = sqrt(numLocales) : int;
const blkSize = n / localesAcross : int;

// can we change WrappedArray to a record?
class WrappedArray {
    proc init() { }

    proc init(row, col, numRows, numCols) {
        dom = {row..row+numRows-1, col..col+numCols-1};
    }

    proc this(i,j : int) ref { return data[i,j]; }

    var dom : domain(2);
    var data : [dom] int;
}



proc luLikeMultiply(
    myLocales  : [?localesDom] locale,
    A          : [localesDom] unmanaged WrappedArray,
    aLocales   : subdomain(localesDom),
    bLocales   : subdomain(localesDom),
    solLocales : subdomain(localesDom))
{
    var rowCopies : [solLocales] owned WrappedArray?;
    var colCopies : [solLocales] owned WrappedArray?;

    // initialize row and col copies
    coforall (locRow, locCol) in solLocales do on myLocales[locRow,locCol] {
        rowCopies[locRow, locCol] = new WrappedArray(
            (locRow-1)*blkSize+1, 1, blkSize, blkSize);
        colCopies[locRow, locCol] = new WrappedArray(
            1, (locCol-1)*blkSize+1, blkSize, blkSize);
    }

    // there are more clever ways of doing these broadcasts, discussed at:
    // http://www.netlib.org/benchmark/hpl/algorithm.html

    // broadcast rows
    forall (locRow, locCol) in aLocales do on myLocales[locRow,locCol] {
        // broadcast a region to other locales on the same row
        forall col in solLocales.dim(2) {
            rowCopies[locRow, col]!.data[A[locRow, locCol].dom] =
                A[locRow,locCol].data;
        }
    }
    // broadcast rows
    forall (locRow, locCol) in bLocales do on myLocales[locRow,locCol] {
        // broadcast b region to other locales on the same col
        forall row in solLocales.dim(1) {
            colCopies[row, locCol]!.data[A[locRow, locCol].dom] =
                A[locRow,locCol].data;
        }
    }

    // do local matrix-multiply
    forall (locRow, locCol) in solLocales do on myLocales[locRow,locCol] {
        ref localA   = rowCopies[locRow,locCol]!.data;
        ref localB   = colCopies[locRow,locCol]!.data;
        ref localSol = A[locRow,locCol].data;

        forall i in localSol.domain.dim(1) {
            forall j in localSol.domain.dim(2) {
                localSol[i,j] = 0;
                for k in localA.domain.dim(2) {
                    localSol[i,j] += localA[i,k] * localB[k,j];
                }
            }
        }
    }
}




proc matrixMult_ijk(
    outerDim,
    innerDim,
    const A : [outerDim, innerDim] int,
    const B : [innerDim, outerDim] int,
    C : [outerDim, outerDim] int)
{
    for i in outerDim {
        for j in outerDim {
            C[i,j] = 0;
            for k in innerDim {
                C[i,j] += A[i,k] * B[k,j];
            }
        }
    }
}

proc main() {
    // verify that the number of locales is a power of 2.
    assert(log2(numLocales) == log2(numLocales):int,
        "Number of locales must be a power of 2");

    // ensure the matrix will divide nicely into processor size chunks.
    assert(blkSize * localesAcross == n,
        "Matrix size must be divisible by sqrt(numLocales)");

    // allocate 2D mesh of locales
    const myLocalesDomain = {1..localesAcross, 1..localesAcross};
    var myLocales : [myLocalesDomain] locale =
      forall (i,j) in myLocalesDomain do
        Locales[(i-1) * localesAcross + (j-1)];

    // Initialize array
    var A : [myLocalesDomain] unmanaged WrappedArray =
      forall (i,j) in myLocalesDomain do createWrappedArray(i,j);

    proc createWrappedArray(i,j) {
      var retval: unmanaged WrappedArray?;
      on myLocales[i,j] {
        const Aij = new unmanaged WrappedArray(
                (i-1)*blkSize+1, (j-1)*blkSize+1, blkSize, blkSize);

        forall (locRow, locCol) in Aij.dom {
                Aij[locRow, locCol] = locRow + locCol;
        }

        retval = Aij;
      }
      return retval!;
    }

    var aLocales   : subdomain(myLocales.domain) = {2..localesAcross, 1..1};
    var bLocales   : subdomain(myLocales.domain) = {1..1, 2..localesAcross};
    var solLocales : subdomain(myLocales.domain) =
        {2..localesAcross, 2..localesAcross};

    // Perform the multiplication in parallel
    luLikeMultiply(myLocales, A, aLocales, bLocales, solLocales);

    // Perform the multiplication using a serial algorithm we know works
    var aRegion   :domain(2) = {1+blkSize..n, 1..1+blkSize-1};
    var bRegion   :domain(2) = {1..1+blkSize-1, 1+blkSize..n};
    var solRegion :domain(2) = {1+blkSize..n, 1+blkSize..n};

    var data : [1..n, 1..n] int;
    forall (i,j) in data.domain do {
        data[i,j] = i+j;
    }

    matrixMult_ijk(
        1+blkSize..n,
        1..1+blkSize-1,
        data(aRegion),
        data(bRegion),
        data(solRegion));

    // compare the solution as calculated by the parallel algorithm against the
    // solution from the serial algorithm
    var passed = true;
    forall (i,j) in myLocales.domain with (ref passed) { // race when "&= true"
        var slice = A[i,j].dom;

        if & reduce(A[i,j].data == data(slice)) then {
            passed &= true;
        } else {
            passed &= false;
        }
    }

    if passed then writeln("PASSED"); else writeln("FAILED");

    for a in A do delete a;
}

