/***********************************************************************
 * Chapel implementation of "99 bottles of beer"
 *
 * by Brad Chamberlain and Steve Deitz
 *
 * Original version written 07/13/2006 in Knoxville airport while
 * waiting for flight home from HPLS workshop.  Updated since then to
 * reflect syntax changes and language improvements.
 *
 * For more information, see https://chapel-lang.org
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
  var count = if bottleNum > 0 then bottleNum:string
              else if startOfVerse then "No more"
              else "no more";
  return count + " bottle" + (if (bottleNum != 1) then "s" else "") + " of beer";
}


//
// Compute whether we'll go to the store or take another one down
//
proc computeAction(bottleNum) {
  return if bottleNum == 0 then "Go to the store and buy some more, "
                           else "Take one down and pass it around, ";
}
