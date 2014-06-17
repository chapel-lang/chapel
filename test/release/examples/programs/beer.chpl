/***********************************************************************
 * Chapel implementation of "99 bottles of beer"
 *
 * by Brad Chamberlain and Steve Deitz
 *
 * Original version written 07/13/2006 in Knoxville airport while
 * waiting for flight home from HPLS workshop.
 *
 * For more information, contact: chapel_info@cray.com
 *
 * modified 08/02/2006 to remove a workaround in the original version
 * modified 08/16/2006 to update syntax (fun -> def)
 * modified 06/12/2007 to clean up for June 2007 release: added main()
 *                     and some comments
 *
 * Notes: 
 * o as in all good parallel computations, boundary conditions
 *   constitute the vast bulk of complexity in this code (invite Brad to
 *   tell you about his zany boundary condition simplification scheme)
 * o uses type inference for variables, arguments
 * o relies on integer->string coercions
 * o uses named argument passing (for documentation purposes only)
 ***********************************************************************/

// allow executable command-line specification of number of bottles 
// (e.g., ./a.out -snumBottles=999999)
config const numBottles = 99;

// the program's entry point
proc main {
  // the number of verses to compute
  const numVerses = numBottles+1;

  // a domain to describe the space of lyrics
  var LyricsSpace: domain(1) = {1..numVerses};

  // array of lyrics
  var Lyrics: [LyricsSpace] string;

  // parallel computation of lyrics array using a forall expression
  //   "forall verse in LyricsSpace, Lyrics(verse) = ..."
  [verse in LyricsSpace] Lyrics(verse) = computeLyric(verse, numVerses);

  // as in any good parallel language, I/O to stdout is serialized.  ;)
  // (Note that I/O to a file could be parallelized using a parallel
  // prefix computation on the verse strings' lengths with file seeking)
  writeln(Lyrics);
}


// HELPER FUNCTIONS:

proc computeLyric(verseNum, numVerses) {
  var bottleNum = numBottles - (verseNum - 1);
  var nextBottle = (bottleNum + numVerses - 1)%numVerses;
  return "\n" // disguise space used to separate elements in array I/O
       + describeBottles(bottleNum, startOfVerse=true) + " on the wall, "
       + describeBottles(bottleNum) + ".\n"
       + computeAction(bottleNum)
       + describeBottles(nextBottle) + " on the wall.\n";
}


proc describeBottles(bottleNum, startOfVerse = false) {
  var bottleDescription = if (bottleNum) then bottleNum:string
                                         else (if startOfVerse then "N" 
                                                               else "n") 
                                              + "o more";
  return bottleDescription 
       + " bottle" + (if (bottleNum == 1) then "" else "s") 
       + " of beer";
}


proc computeAction(bottleNum) {
  return if (bottleNum == 0) then "Go to the store and buy some more, "
                             else "Take one down and pass it around, ";
}
