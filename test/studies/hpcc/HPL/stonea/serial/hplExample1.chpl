// This is the version of HPL I went over with Brad and Steve in my weekly
// meeting on July 8, 2008. This version is serial, blocked, and includes
// pivoting. It does not not include method used by HPL to verify results.  It
// includes a number of testing functions to verify the implementation, these
// functions are executed when the program is run.
//
// This version uses the counted ranges feature we've been talking about (which
// allows for range literals such as 3..#5). Included in this directory is a
// patch to enable this feature.  To apply the patch move it to the $CHPL_HOME
// directory, execute "patch -p0 < countedRange.patch", then recompile.
//
// Some features of this version:

// Some things we talked about in the meeting:
//  - I implemented the panel2D structure so that I'd have some way of
//    passing a submatrix to a function where the submatrix was cognizant
//    about what matrix it's embedded in. This is necessary for the panelSolve
//    function which does a computation in a submatrix, but also needs to
//    permute the rows of the full-matrix the submatrix is inside of.
//    It's questionable whether this technique really improves readability of
//    the code, so I'm going to try modifying this code to not have Panel2D
//    and see how it goes.
//  - I'm unsure if the failure rate I'm getting for LU decomposition
//    (about 20% is due to numerical stability issues or a bug in the
//    algorithm).

use Random;

config const seed = 31415;

record Panel2D {
    var matrix;
    const rowOffset : int;
    const colOffset : int;
    const rows : int;
    const cols : int;

    proc panel {
        ref pnl : [1..rows, 1..cols] =
            matrix[rowOffset..rowOffset+rows-1, colOffset..colOffset+cols-1].reindex(1..rows, 1..cols);
        return pnl;
    }

    //ref panel =
        //matrix[rowOffset..rowOffset+rows-1, colOffset..colOffset+cols-1].reindex(1..rows, 1..cols);
};


// calculate C = C - A * B.
proc dgemm(A : Panel2D, B : Panel2D, C : Panel2D)
{
    assert(A.cols == B.rows);
    assert(C.rows == A.rows);
    assert(C.cols == B.cols);

    // Calculate (i,j) using a dot product of a row of A and a column of B.
    for i in 1..#C.rows {
        for j in 1..#C.cols {
            for k in 1..#A.cols {
                C.panel[i,j] -= A.panel[i, k] * B.panel[k, j];
            }
        }
    }
}


// do unblocked-LU decomposition within the specified panel, update the
// pivot vector accordingly
proc panelSolve(pnl : Panel2D, piv : [1..pnl.rows] int)
{
    ref matrix = pnl.matrix;
    ref panel  = pnl.panel;

    // iterate through the columns
    for k in 1..pnl.cols {
        ref col = panel[k.., k];

        // The pivot is the element with the largest absolute value.  Note, I
        // can't assign pivot in the maxloc expression, something like: (pivot,
        // pivotrow) = maxloc ... because this would assign pivot to the
        // absolute value of the pivot element, not the actual value.
        var (_, pivotRow) = maxloc reduce(abs(col), col.domain);
        var pivot = col[pivotRow];

        // Swap the current row with the pivot row
        piv[k] <=> piv[pivotRow];
        matrix[pnl.rowOffset+k-1, ..] <=>
            matrix[pnl.rowOffset+pivotRow-1, ..];
        
        if(pivot == 0) then halt("Matrix can not be factorized");
        
        // divide all values below and in the same col as the pivot by
        // the pivot
        if k+1 <= pnl.rows {
            col[k+1..] /= pivot;
        }

        // update all other values below the pivot
        if k+1 <= pnl.rows && k+1 <= pnl.cols {
            ref pnlBlwPiv = panel[k+1.., k+1..];
            forall (i,j) in pnlBlwPiv.domain {
                panel[i,j] -= panel[i,k] * panel[k,j];
            }
        }
    }
}

// Update the block row portion of the matrix in a blocked LU decomposition.
// Each step of the LU decomposition will solve for a submatrix using an
// unblocked algorithm. This function solves the rows to the right of the
// block.
proc updateBlockRow(blockPnl : Panel2D, blockRow : Panel2D) {
    assert(blockPnl.rows == blockPnl.cols);
    const ref block = blockPnl.panel;
    const ref panel = blockRow.panel;

    for i in 1..blockRow.rows {
        forall j in 1..blockRow.cols {
            for k in 1..i-1 {
                panel[i, j] -= block[i, k] * panel[k,j];
            }
        }
    }
}

// blocked LU factorization with pivoting for matrix augmented with vector of
// RHS values.
proc LUFactorize(n : int, A : [1..n, 1..n+1] real, piv : [1..n] int) {
    param blkSize = 3;

    forall i in 1..n do
        piv[i] = i;
    
    for blk in 1..n by blkSize {
        var pnlSize = min(blkSize, n-blk+1);
        var remaining = n-blk-pnlSize+1;
        
        var tlPnl = new Panel2D(A, blk, blk, pnlSize, pnlSize);
        var blPnl = new Panel2D(A, blk+pnlSize, blk, remaining, pnlSize);
        var lPnl  = new Panel2D(A, blk, blk, n-blk+1, pnlSize);
        var trPnl = new Panel2D(A, blk, blk+pnlSize, pnlSize, remaining+1);
        var brPnl = new Panel2D(A, blk+pnlSize, blk+pnlSize, remaining,
                                remaining+1);

        panelSolve(lPnl, piv[blk..n]);
        updateBlockRow(tlPnl, trPnl);

        // update trailing submatrix (if there)
        if(brPnl.panel.domain.numIndices > 0) {
            dgemm(blPnl, trPnl, brPnl);
          }
      }
}

// -------------------------------------------------------------------------- 
//   TESTING SYSTEM:
// -------------------------------------------------------------------------- 

proc matrixMult(
    const m : int,
    const p : int,
    const n : int,
    const A : [1..m, 1..p],
    const B : [1..p, 1..n],
    C : [1..m, 1..n])
{
    C = 0;

    forall (i,j,k) in {C.domain.dim(1), C.domain.dim(2), 1..p} {
        C[i,j] += A[i,k] * B[k,j];
    }
}

// given a matrix in A in form: [L, U] multiply the L and U parts into
// a resulting matrix C.
proc selfMult(n : int, A : [1..n,1..n] real, C : [1..n,1..n] real) {
    C = 0;

    forall (i,j) in C.domain {
        if(i <= j) {
            for k in 1..i-1 {
                C[i,j] += A[i,k] * A[k,j];
            }
            C[i,j] += A[i,j];
        }
        else if(j <= i){
            for k in 1..j {
                C[i,j] += A[i,k] * A[k,j];
            }
        }
    }
}

// given a matrix and a permuatations vector apply the permutations
// QUESTION: I'm intending vectorIn to be passed by value in this instance
// (since I modify it in the function but I don't want the result sent out).
// is this function doing the trick?
proc permuteMatrix(matrix : [?dmn], in vector) {
    //var pdmn : sparse subdomain(dmn);
    var pdmn =
        {1..vector.domain.dim(1).size, 1..vector.domain.dim(1).size};
    var p : [pdmn] int;
    //p.IRV = 0;

    // construct permutation matrix
    forall i in vector {
        //pdmn.add((i, vector[i]));
        p[i, vector[i]] = 1;
    }

    // multiply permutation matrix times the matrix
    var permuted = matrix;

    matrixMult(
        dmn.dim(1).size,
        dmn.dim(1).size,
        dmn.dim(2).size,
        p, matrix, permuted);

    matrix = permuted;
}

proc permuteBack(matrix : [?dmn], in vector) {
    // this just does a bubblesort of the permutation vector swapping the
    // rows of the matrix alongside. Yes the performance here will be horrible,
    // not to mention all the data being shuffled around, but this function
    // is in the test system so it's excusable.

    const low  = vector.domain.dim(1).low;
    const high = vector.domain.dim(1).high;

    for i in vector.domain {
        for j in low..high-1 {
            if vector[j+1] < vector[j] {
                vector[j] <=> vector[j+1];
                matrix[j, ..] <=> matrix[j+1, ..];
            }
        }
    }
}

proc test_permuteMatrix(rprt = true) : bool {
    // test one of the testing functions: permuteMatrix (is this getting a
    // little too meta here?)

    param n = 10;
    var rand = new RandomStream(seed);

    // this n by n array is filled so each element is assigned to its
    // row number. This test will permute these elements, keeping a pivot
    // vector alongside.
    var A : [1..n, 1..n] int = [(i,j) in {1..n, 1..n}] i;
    var piv : [1..n] int = [i in 1..n] i;
    var AOrig = A;

    // do 100 random swaps of A
    for i in 1..100 {
        var randRow1 = (rand.getNext() * 10000):int % n + 1;
        var randRow2 = (rand.getNext() * 10000):int % n + 1;

        A[randRow1, ..] <=> A[randRow2, ..];
        piv[randRow1] <=> piv[randRow2];
    }

    // permute original version of A
    permuteMatrix(AOrig, piv);

    // calculate and report results. Calculate by comparing the original
    // version of A which we just permuted to swapped version.
    if rprt then write("test_permuteMatrix: ");
    if & reduce(AOrig == A) then {
        if rprt then writeln("PASSED");
        return true;
    } else {
        if rprt then writeln("FAILED");
        return false;
    }
}

proc test_panelSolve(rprt = true) : bool {
    var rand = new RandomStream();

    var piv : [1..8] int = [i in 1..8] i;
    var A : [1..8, 1..9] real =
        [(i,j) in {1..8, 1..9}] (rand.getNext() * 10000):int % 100 + 1;
    var AOrig = A;

    var AOrig2 = A;

    // grab a panel and solve it
    param offset = 3;
    param blkSize = 3;
    var pnl = new Panel2D(
        A, offset, offset, (A.domain.dim(1).size)-offset+1, blkSize);
    panelSolve(pnl, piv[offset..]);

    // to test multiply the L and U parts of the top portion of the panel
    // (just the block) together and check that the result matches the
    // block on the original matrix when permuted.
    var C : [1..blkSize, 1..blkSize] real;
    var blockD =
        {offset..#blkSize, offset..#blkSize};
    selfMult(blkSize, A(blockD), C);
    permuteMatrix(AOrig, piv);

    // calculate and report results.
    if rprt then write("test_panelSolve: ");
    // NOTE: The reduce statement requires this heavy parenthesization,
    // if I just do "if(& reduce AOrig(blockD) == C) {" it'll cause a compiler
    // error.
    if(& reduce ((AOrig(blockD)) - C <= .001)) {
        if rprt then writeln("PASSED");
        return true;
    } else {
        if rprt then writeln("FAILED");
        return false;
    }
}

proc test_updateBlockRow(rprt = true) : bool {
    var rand = new RandomStream();

    // construct a matrix A = [X | Y], where X is an already LU-factorized
    // submatrix and Y is the block row we wish to update and test
    var randomOffset : int = (rand.getNext() * 100):int;
    var randomHeight : int = (rand.getNext() * 100):int + 1;
    var randomWidth  : int = (rand.getNext() * 100):int + randomHeight;
    var A : [randomOffset..#randomHeight,
             randomOffset..#randomWidth] real;
    rand.fillRandom(A);
    var OrigA = A;
    
    // capture X and Y
    var X = new Panel2D(
        A, randomOffset, randomOffset, randomHeight, randomHeight);
    var Y = new Panel2D(
        A, randomOffset, randomOffset+randomHeight, randomHeight,
        randomWidth - randomHeight);

    // apply the update block row function
    updateBlockRow(X, Y);

    // do a matrix multiply of L and U to obtain the original values in
    // the block row
    var recalc : [randomOffset..#randomHeight,
                  randomOffset+randomHeight..(randomWidth-randomHeight)] real;
    forall (i,j) in recalc.domain {
        for k in randomOffset..i-1 {
            recalc[i,j] += A[i,k] * A[k,j];
        }
        recalc[i,j] += A[i,j];
    }

    // calculate and report results
    if rprt then write("test_updateBlockRow: ");
    if(& reduce (abs(recalc - (OrigA(recalc.domain))) <= 0.1)) {
        if rprt then writeln("PASSED");
        return true;
    }
    else {
        if rprt then writeln("FAILED");
        return false;
    }
}

proc test_LUFactorize(rprt = true) : bool {
    var rand = new RandomStream();

    var randomN : int = (rand.getNext() * 10):int + 1;
    var A : [1..randomN, 1..randomN+1] real;
    fillRandom(A);
    var origA = A;

    var piv : [1..randomN] int;

    LUFactorize(randomN, A, piv);
    
    var C : [1..randomN, 1..randomN] real;
    selfMult(randomN, A[1..randomN,1..randomN], C);
    permuteBack(C, piv);

    if rprt then write("test_LUFactorize: ");
    if(& reduce (abs(C - origA(1..randomN, 1..randomN)) <= 0.01)) {
        if rprt then writeln("PASSED");
        return true;
    } else {
        if rprt then writeln("FAILED");
        return false;
    }
}

proc main() {
    var result = true;
    write("test_permuteMatrix: ");
    for i in 1..100 do
        result &= test_permuteMatrix(rprt=false);
    if result then writeln("PASSED");
    else writeln("FAILED");

    var numPassed = 0;
    write("test_panelSolve: ");
    for i in 1..1000 do
        numPassed += test_panelSolve(rprt=false);
    writeln(numPassed, " PASSED, ", 1000-numPassed, " FAILED");

    numPassed = 0;
    write("test_updateBlockRow: ");
    for i in 1..100 do
        numPassed += test_updateBlockRow(rprt=false);
    writeln(numPassed, " PASSED, ", 100-numPassed, " FAILED");

    numPassed = 0;
    write("test_LUFactorize: ");
    for i in 1..1000 do
        numPassed += test_LUFactorize(rprt=false);
    writeln(numPassed, " PASSED, ", 1000-numPassed, " FAILED");
}

// Sample output:
// test_permuteMatrix: PASSED
// test_panelSolve: 1000 PASSED, 0 FAILED
// test_updateBlockRow: 100 PASSED, 0 FAILED
// test_LUFactorize: 1000 PASSED, 0 FAILED

