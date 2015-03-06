/***********************************************************************
 * Chapel implementation of "99 bottles of beer"
 *
 * by Brad Chamberlain and Steve Deitz
 *
 * Original version written 07/13/2006 in Knoxville airport while
 * waiting for flight home from HPLS workshop.  Updated since then to
 * reflect syntax changes and language improvements.
 *
 * For more information, see http://chapel.cray.com
 *
 * Notes: 
 * o as in all good parallel computations, boundary conditions
 *   constitute the vast bulk of complexity in this code
 * o uses type inference for most variables, arguments
 * o uses named argument passing for documentation purposes
 ***********************************************************************/

//
// Support executable command-line specification of number of bottles 
// (e.g., ./beer --numBottles=999999)
//
config const numBottles = 99;


//
// Global constants and variables
//
const numVerses = numBottles+1;       // the number of verses to compute

const LyricsSpace  = {1..numVerses};  // the index set for the lyrics
var Lyrics: [LyricsSpace] string;     // array of lyrics


//
// compute the array of lyrics in parallel using a forall loop
//
forall verse in LyricsSpace do
  Lyrics[verse] = computeLyrics(verse);


//
// print the lyrics out to the console
//
writeln(Lyrics);


// ------------ Helper functions -------------

//
// Compute the lyrics for the verse #verseNum
//
proc computeLyrics(verseNum) {
  const bottleNum = numBottles - verseNum + 1,               // what are we on?
        nextBottle = (bottleNum + numVerses - 1)%numVerses;  // what's next?

  return "\n"
        + describeBottles(bottleNum, startOfVerse=true) + " on the wall, "
        + describeBottles(bottleNum) + ".\n"
        + computeAction(bottleNum)
        + describeBottles(nextBottle) + " on the wall.\n";
}


//
// Given a bottle number and whether or not we're at the start of the
// verse, generate the string "# bottles of beer on the wall" where
// # should result in "[N/n]o more" for zero bottles.
//
proc describeBottles(bottleNum, startOfVerse = false) {
  // Passing the result of a function call by (modifiable) ref to another function requires
  // that it first be captured in a named variable.
  // In this respect, Chapel differs from C++, which allows an lvalue reference
  // to bind to an rvalue [ISO/IEC14882:1998, 8.5.3].

  // Following this revision, toString(ref csc:c_string_copy) and
  // variations of proc + (concatenation) take their c_string_copy
  // arguments by reference, so temporary c_string_copy results must
  // be stored in a variable.  In addition to making toString happy, it allows
  // the concatenation routines to be written in a way that they do not leak
  // memory.  One can consider a ref c_string_copy argument to be the equivalent of
  // an rvalue reference.
  // This change is only temporary: When rvalue
  // references are implemented, then it should be legal to bind an rref to the
  // result of a function, so the need for named temporaries goes away.
  var count:string;
  if bottleNum then count = bottleNum:string;
  else {
    var first = (if startOfVerse then "N" 
                                 else "n") + "o more";
    count = first;
  }
  return count + " bottle" + (if (bottleNum != 1) then "s" else "") + " of beer";
}


//
// Compute whether we'll go to the store or take another one down
//
proc computeAction(bottleNum) {
  return if (bottleNum == 0) then "Go to the store and buy some more, "
                             else "Take one down and pass it around, ";
}
