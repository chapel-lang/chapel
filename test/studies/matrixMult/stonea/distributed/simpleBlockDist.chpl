/* To simplify matters I assume the number of locales is a power of 2,
   the LHS and RHS matrices are squares, and the matrix will divide nicely into
   processor size chunks.
*/

use Math;

config const n = 10;
const localesAcross = sqrt(numLocales) : int;
const blkSize = n / localesAcross : int;

record WrappedArray {
    proc init() { }

    proc init(row, col, numRows, numCols) {
        dom = {row..row+numRows-1, col..col+numCols-1};
    }

    proc this(i,j : int) ref { return data[i,j]; }

    var dom : domain(2);
    var data : [dom] int;
}


proc simpleDistMultiply(
    blkSize : int,
    A : [?localesDom] WrappedArray,
    B : [localesDom] WrappedArray,
    C : [localesDom] WrappedArray,
    myLocales : [localesDom] locale)
{
    // Every locale needs a copy of the blocks of A in the same locale-row and
    // the blocks of B in the same locale-column.
    var rowCopies : [localesDom] WrappedArray;
    var colCopies : [localesDom] WrappedArray;

    // initialize row and col copies
    coforall (locRow, locCol) in localesDom do on myLocales[locRow,locCol] {
        rowCopies[locRow, locCol] = new WrappedArray(
            (locRow-1)*blkSize+1, 1, blkSize, n);
        colCopies[locRow, locCol] = new WrappedArray(
            1, (locCol-1)*blkSize+1, n, blkSize);
    }

    // broadcast rows and columns
    forall (locRow, locCol) in localesDom do on myLocales[locRow,locCol] {
        // broadcast A to other locales on the same row
        forall col in localesDom.dim(1) {
            rowCopies[locRow, col].data[A[locRow, locCol].dom] =
                A[locRow,locCol].data;
        }

        // broadcast B to other locales on the same column
        forall row in localesDom.dim(0) {
            colCopies[row, locCol].data[B[locRow, locCol].dom] =
                B[locRow,locCol].data;
        }
    }

    // do local matrix-multiply
    forall (locRow, locCol) in localesDom do on myLocales[locRow,locCol] {
        ref localA = rowCopies[locRow,locCol].data;
        ref localB = colCopies[locRow,locCol].data;
        ref localC = C[locRow,locCol].data;

        forall i in localC.domain.dim(0) {
            forall j in localC.domain.dim(1) {
                for k in localA.domain.dim(1) {
                    localC[i,j] += localA[i,k] * localB[k,j];
                }
            }
        }
    }
}


proc main() {
    // verify that the number of locales is a power of 2.
    // QUESTION: How safe is it really to do this? Is there a possibility
    // of some kind of roundoff error? Another method would be to see if
    // only one bit is set to '1' in numLocales.
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

    // Initialize A, B, and C arrays
    var A : [myLocales.domain] WrappedArray;
    var B : [myLocales.domain] WrappedArray;
    var C : [myLocales.domain] WrappedArray;
    forall (i,j) in myLocales.domain do on myLocales[i,j] {
        cobegin {
            A[i,j] = new WrappedArray(
                (i-1)*blkSize+1, (j-1)*blkSize+1, blkSize, blkSize);
            B[i,j] = new WrappedArray(
                (i-1)*blkSize+1, (j-1)*blkSize+1, blkSize, blkSize);
            C[i,j] = new WrappedArray(
                (i-1)*blkSize+1, (j-1)*blkSize+1, blkSize, blkSize);
        }

        forall (locRow, locCol) in A[i,j].dom {
            cobegin {
                A[i,j][locRow, locCol] = locRow + locCol;
                B[i,j][locRow, locCol] = locRow * locCol;
            }
        }
    }

    // Perform the multiplication
    simpleDistMultiply(blkSize, A, B, C, myLocales);

    // Write results (the contents of C)
    writeln("Results: ");
    for i in 1..n {
        for j in 1..n {
            if j != 1 then write(" ");
            write(C[((i-1)/blkSize)+1, ((j-1)/blkSize)+1][i,j]);
        }
        writeln();
    }
}
