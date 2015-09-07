/*  Slicing Primer    

    This example program demonstrates the use of array slicing and
    reindexing.  

    In this program, diagonal blocks (or slices) of A are initialized
    according to the row indices of the diagonal block.  
       A(i,j) = i, for all (i,j) in diagonal blocks
    If the block has been reindexed, then each diagonal block is 
    initialized with the same values.  If the block has not been 
    reindexed, then the block has the same indices as the full array, 
    so each diagonal block is initialized with different values.

    (Incomplete diagonal blocks are not initialized.  If blk does not
    divide into n evenly, the last diagonal block of A will be 
    uninitialized.)

    Three different cases are given, to show different ways of slicing
    arrays, with and without reindexing.  Array aliases are declared 
    for the diagonal blocks in two of the three cases.  One of these 
    array aliases uses reindexing of the domain of the array alias.
    The initialization of the blocks is handled in either initBlock
    or initBlock2.  The procedure initBlock does not reindex the domain
    of the input array while initBlock2 does.
*/

// Configuration variables for n and blk where the array A is nxn and
// blk is the size of the diagonal subblock. 
config const n = 10,
             blk = 2;

proc main() {

  // Variable declarations for the range vec, and for the domain D
  // and array A.  Vec is used to define the ranges of each dimension
  // of D and it is later used in the iterator for the for loop statements
  // which call the init procedure for each diagonal block of A.
  const vec = 1..n;
  var D = {vec,vec};
  var A:[D] int;

  // D0 is a domain which is used to reindex the diagonal blocks of A.
  var D0 = {1..blk, 1..blk};

  // Each of the following three for loop statements uses the
  // iterator blockIter to yield a range subvec that is then
  // used to define (subvec x subvec) blocks along the diagonal 
  // of A.

  //  Case 1:  No reindexing of the diagonal blocks.
  //  The array slice is sent directly to initBlock.
  writeln("Initializing the diagonal blocks of A.");
  writeln("No reindexing of diagonal blocks.");
  for subvec in blockIter(vec,blk) {
    initBlock(A[subvec,subvec]);
  }

  writeln(A);
  writeln();

  // A is reset to zero between each case.  (This step is not
  // necessary, but done to show that each case starts with a zero array.)
  A = 0;

  // Case 2:  An array alias with reindexing is used to point to each
  // diagonal block, and that array alias is sent to initBlock.
  writeln("Initializing the diagonal blocks of A."); 
  writeln("Reindexing of each diagonal block in alias declaration. ");
  for subvec in blockIter(vec,blk) {
    var Ablock: [D0] => A[subvec,subvec];
    initBlock(Ablock);
  }

  writeln(A);
  writeln();

  A = 0; 

  // Case 3: An array alias is used to point to each diagonal block
  // and is reindexed to use 1-based indexing in order to meet the
  // requirements of the initBlock2() routine.
  writeln("Initializing the diagonal blocks of A."); 
  writeln("Reindexing of array argument in init procedure definition. ");
  for subvec in blockIter(vec,blk) {
    var Ablock => A[subvec,subvec].reindex({1..blk, 1..blk});
    initBlock2(Ablock);
  }

  writeln(A);
  writeln();
}

// The iterator blockIter yields a range that defines a subset
// of vec, which is always of length blk.  
iter blockIter(vec:range,blk) {
  for i in vec by blk {
    if (i + blk-1 <= vec.high) then 
      yield i..i+blk-1;
  }
}

// This procedure sets each element of A to be the value of its
// row index.
proc initBlock(A) {
  for (i,j) in A.domain {
    A(i,j) = i;
  }
}

// This procedure requires the domain of the array argument to
// be [1..blk,1..blk].  It sets each element of A to be the value of
// its row index.
proc initBlock2(A: [1..blk,1..blk]) {
  for (i,j) in A.domain {
    A(i,j) = i;
  }
}
