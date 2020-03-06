use HDFSiterator, HDFStools, IO;

// The actual mappers

var BlockSpace: domain(int);
var reducerArray: [BlockSpace] (int, int, int); // (beginWhite, wc, endWhite)

writeln();
writeln("========== Starting word-count test =============");
writeln();

forall s in HDFSmap("/tmp/test.txt") {
  wordCount(s);
} 

HDFSreduce();

// return a tuple (startWhite, wc, endWhite)
proc wordCount((s_c_string, block)) {
  var s = s_c_string: string;
  var nl = 0; // number of lines
  var nw = 0; // number of words
  var IN = 1; // in a word
  var OUT = 0; // out of a word
  var state = OUT; // start out of a word
  var startWhite = 0; // did we start with whitespace?
  var endWhite   = 0; // end with whitespace?
  var sss = s[1];

  // We finished the last blocks word
  if (sss == " " || sss == "\n")
    then startWhite = 1;

  // We've ended the word and/or line
  sss = s[s.size];
  if (sss == " " || sss == "\n")
    then endWhite = 1;

  for incr in 1..#s.size { 
  var ss = s[incr];

    if (ss == "\n")
      then nl += 1;
    if (ss == " " || ss == "\n" || ss == "\t")
      then state = OUT;
    else if (state == OUT) {
      state = IN;
      nw += 1;
    }
  }

  BlockSpace.add(block);
  reducerArray[block] = (startWhite, nw, endWhite);
  writeln("wordCount counted: ", nl, " lines ", nw, " words on locale"+ here.name + " block ", block, " tuple: ", (startWhite, nw, endWhite)); 
}

// ============ Reduction steps ===================
proc HDFSreduce() {
  var iterTo = max reduce BlockSpace; // get the max number of our blocks
  var total = 0; // assume no overlapping words to start
  var accum = reducerArray[0](2); // Get base case -- We can always be assured that
                                  // we always have at least one block
  if (iterTo >= 1) { // We have more then 1 block (blocks are 0 indexed)
    for i in 0..iterTo-1 {
      var tmpOne = reducerArray[i];
      var tmpTwo = reducerArray[i+1];

      // If we have an overlapping word, subtract one from the total count
      // Since we've double counted
      if ((tmpOne(3) | tmpTwo(1)) == 0)  // case: x][x
        then total -= 1;

      accum += tmpTwo(2);
    }
  }
  writeln("number of overlaps: ", 0-total);
  writeln("total words are: ", accum + total);
}

///============================ Scratch ====================




























var numRead: int;
config const recordStart = "beer/name:";
config const recordEnd = "\n\n";

var acc: BeerAdvocateRecord;
var base = 1;
var BeerNameSpace: domain(string);
var numReviewsLocal: [BeerNameSpace] int;












// Function to map
proc myMapFn((s, block)) {
  do {
    var start = indexOf(recordStart, s, base);

    if (start == -1)  // done
      then break;

    var end = indexOf("beer/beerId:", s, start + recordStart.size);
    if (end == -1) { // then close to EOF
      end = s.size;
      base = end;
    } else base = end; 

    var ss = s[start..end];

    var (beerName, ignore) = findFieldValue(ss, "beer/name: ", 1); 

    BeerNameSpace.add(beerName);
    numReviewsLocal[beerName] += 1;
  } while (true);
}

proc reduceMyMapFn() {
  var acc = 0;
  for beerName in BeerNameSpace {
    writeln("beer: "+ beerName + " has ", numReviewsLocal[beerName], " reviews");
  }
}


proc mapFn(s: string) {
  do {
    // ==== Get the start of the review we want to parse
    var i = indexOf(recordStart, s, base);

    // ==== if we cant, say so and exit
    if (i == -1) {
      //writeln("Couldn't find start of record");
      break;
    }

    // Get the end of the review as well
    var j = indexOf(recordEnd, s, i + recordStart.size);

    if (j == -1) {
      // Find records at end of file
      j = s.size;
      base = j;
    } else base = j;

    // ==== Get the length (in characters) of the review in question
    var recordLength = (j - i + 1);
    // ==== Split what you read in, so now we have ss = review in question
    // Include the final field delimiter
    var ss = s[i..j];
    //writeln("Found record with start " + i + ", end " + (j - 1) + ", length " + recordLength + ": ");

    // Deserialize structured record from text
    // ==== Read review in to be a record 
    var rec = deserializeRecord(ss);
    // ==== Increment the number we have read by 1
    numRead += 1;
    //writeln(here.id + " read " + numRead + " records in block " + block);

    // ==== Record observation(s) in accumlator(s)
    BeerNameSpace.add(rec.beer_name);
    numReviewsLocal[rec.beer_name] += 1;

  } while(true);


  for  beerName in BeerNameSpace {
    write(beerName + "\t");
    writeln(numReviewsLocal[beerName]);
  }
  writeln("\n");
}


