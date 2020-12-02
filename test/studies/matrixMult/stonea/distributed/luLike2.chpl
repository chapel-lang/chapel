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

config const n = 9;
//const localesAcross = sqrt(numLocales) : int;
const localesAcross = 3 : int;
const numBlocks = localesAcross;
const blkSize = n / numBlocks : int;



proc matrixMult_ijk(
    const m : int,
    const p : int,
    const n : int,
    const A : [?AD] int,
    const B : [?BD] int,
    C : [?CD] int)
{
    for (ai,ci) in zip(AD.dim(0), CD.dim(0)) {
        for (cj,bj) in zip(CD.dim(1), BD.dim(1)) {
            C[ci,cj] = 0;
            for (ak,bk) in zip(AD.dim(1),BD.dim(0)) {
                C[ci,cj] += A[ai,ak] * B[bk,bj];
            }
        }
    }
}

proc luLikeMultiply(A : [1..n, 1..n] int)
{
    var ACopies : [A.domain[blkSize+1..n, blkSize+1..n]] int;
    var BCopies : [A.domain[blkSize+1..n, blkSize+1..n]] int;

    // stamp A across into ACopies,
    forall blkCol in 2..numBlocks {
        var aBlkD : subdomain(ACopies.domain) =
            {blkSize+1..n, 1..blkSize};
        var cBlkD : subdomain(A.domain) =
            {blkSize+1..n, (blkCol-1)*blkSize+1..blkCol*blkSize};

        ACopies[cBlkD] = A[aBlkD];
    }

    // stamp B down into BCopies, 
    forall blkRow in 2..numBlocks {
        var bBlkD : subdomain(BCopies.domain) = 
            {1..blkSize, blkSize+1..n};
        var cBlkD : subdomain(A.domain) =
            {(blkRow-1)*blkSize+1..blkRow*blkSize, blkSize+1..n};
        
        BCopies[cBlkD] = A[bBlkD];
    }

    // do local matrix-multiply on a block-by-block basis
    forall (blkRow,blkCol) in {2..numBlocks, 2..numBlocks} {
        var region : subdomain(A.domain) =
            {(blkRow-1)*blkSize+1..blkRow*blkSize,
             (blkCol-1)*blkSize+1..blkCol*blkSize};
        local {
            matrixMult_ijk(
                blkSize, blkSize, blkSize,
                ACopies(region), BCopies(region), A(region));
        }
    }
}



proc main() {
    // verify that the number of locales is a perfect-square
    assert(sqrt(numLocales) == sqrt(numLocales):int,
        "Number of locales must be a perfect-square");

    // ensure the matrix will divide nicely into processor size chunks.
    assert(blkSize * localesAcross == n,
        "Matrix size must be divisible by sqrt(numLocales)");

    // Initialize array
    var A : [{1..n, 1..n}] int = [(i,j) in {1..n, 1..n}] i+j;
    var ACopy = A;
    
    // Perform the multiplication in parallel
    writeln("Before mult: \n", A);
    luLikeMultiply(A);
    writeln("After mult: \n", A);

    // Perform the multiplication using a serial algorithm we know works
    var aRegion   :domain(2) = A.domain[1+blkSize..n, 1..1+blkSize-1];
    var bRegion   :domain(2) = A.domain[1..1+blkSize-1, 1+blkSize..n];
    var solRegion :domain(2) = A.domain[1+blkSize..n, 1+blkSize..n];

    matrixMult_ijk(
        n-blkSize,
        blkSize,
        n-blkSize,
        ACopy(aRegion),
        ACopy(bRegion),
        ACopy(solRegion));

    // compare the solution as calculated by the parallel algorithm against the
    // solution from the serial algorithm
    var passed = true;
    if & reduce(A == ACopy) then {
        passed = true;
    } else {
        passed = false;
    }

    if passed then writeln("PASSED"); else writeln("FAILED");
}

