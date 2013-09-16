/* General roadmap of this file:
 * Create a file handle and connect to HDFS
 * Get replication, block size and length of file
 * Get the hosts that the blocks of the file resides on
 * Get the blocks
 * Connect to HDFS
 * Open the file 
 * Create an array for each beer name (we have an assoc beername <==> number of
 reviews)
 * For each block
 * Get the host that the block resides on TODO: check this
 * if the locale we're on owns this block (i.e we own it)
 * Fetch the contents of the block
 * Read in characters from the file
 * Get the start of the review we want to parse
 * if we cant, say so and exit
 * Get the length (in characters) of the review in question
 * Split what you read in, so now we have ss = review in question
 * Read review in to be a record 
 * Increment the number we have read by 1
 * Record observation(s) in accumlator(s)
 * Return back the number of reviews for each beer
 ============ Procedures ================
 * IndexOf(ss: string, s: string, start: int) 
 -- Given a string s, we return the index of the 
 first occurence of the substring ss after start.
 * deserializeRecord(s: string) 
 -- Parses a review into its internal representation as a record
 * findFieldValue(s: string, fieldStart: string, base: int) 
 -- Returns a tuple (field, value_of_field). i.e (beer, beerID)
 * findFieldValueReal(s: string, fieldStart: string, base: int) -- Need to talk with Brad about this one...
 * findFieldValueInt(s: string, fieldStart: string, base: int) -- Same here

 ==================== FIXME's  ====================================
 * Make BeerAdvocateRecord smaller (Don't want to do this right now, want to keep code
 more general)
 - Make deserializeRecord not parse every field of the review and put inside a
 record instead just skip past all the ones that we dont care about, and just
 return the beer_ID (talk to Brad to make sure this would be "kosher")
 - Removing deserializeRecord would allso have the efect of eliminating a fair
 amount of code (most likely)


 */
use Sys, HDFS;

config const namenode = "default"; 
config const namenodeNum: c_int = 0;
config const dataFile = "/tmp/ba_sample.txt";
config const domainSuffix = "";
config const blockOverlap = 1024;
config const recordStart = "beer/name:";
config const recordEnd = "\n\n";
config param computeInParallel = false;

/* Problem domain */
record BeerAdvocateRecord {
  var beer_name: string;
  var beer_beerId: int;
  var beer_brewerId: int;
  var beer_ABV: real;
  var beer_style: string;
  var review_appearance: real;
  var review_aroma: real;
  var review_palate: real;
  var review_taste: real;
  var review_overall: real;
  var review_time: int;
  var review_profileName: string;
  var review_text: string;
}


// Put in coforall for distributed memory purposes
// ==== Get replication, block size and length of file
//	var fileInfo = HDFS.chadoopGetFileInfo(hdfsFS, dataFile);
//	writeln("Info for file " + dataFile + ":");
//	writeln(fileInfo);
// ==== Get the hosts that the blocks of the file resides on
//	var blockHosts = HDFS.hdfsGetHosts(hdfsFS, dataFile, 0, fileInfo.mSize);
//  // ==== Get the blocks
//	var blockCount = HDFS.chadoopGetBlockCount(blockHosts);
var localeHosts: [0..#numLocales] string;

for loc in Locales {
  on loc {
    localeHosts[loc.id] = loc.name;
  }
}

//writeln("localeHosts: " + localeHosts);
// Why the file connections need to be inside a coforall:
// When we run it on each locale, we copy the pointer over to that locale,
// therefore when we go to use that pointer on a different locale, it is
// pointing to utter junk. This is why we have to move the connect etc.
// inside th coforall, since this way the pointers reside "locally" on each locale
// and therefore address valid memory on that node.

sync coforall loc in Locales { // look at this more

  on loc {
    // changed for distributed memory
    // To process a file, do the following:
    // 1) Get the file info, which includes replication, block size, and length
    // 2) Find all the hosts/blocks for the file, from byte 0 to length
    // 3) On each locale, find the blocks you own and process them
    //write("Connecting to HDFS on host " + here.name + "...");
    //var hdfsFS: c_void_ptr = HDFS.hdfsConnect("default", 0);
    var hdfsFS: c_void_ptr = HDFS.hdfsConnect(namenode, namenodeNum);
    //writeln("done");
    var fileInfo = HDFS.chadoopGetFileInfo(hdfsFS, dataFile);
    //      writeln("Info for file " + dataFile + ":");
    //      writeln(fileInfo);
    var blockHosts = HDFS.hdfsGetHosts(hdfsFS, dataFile, 0, fileInfo.mSize);
    // ==== Get the blocks
    var blockCount = HDFS.chadoopGetBlockCount(blockHosts);
    // END

    // ==== Connect to HDFS
    // ==== Open the file 
    //writeln("Opening file " + dataFile + " on " + here.name + "...");
    var dataFileLocal = HDFS.hdfsOpenFile(hdfsFS, dataFile, O_RDONLY, 0, 0, 0);
    assert(HDFS.IS_NULL(dataFileLocal) == HDFS.IS_NULL_FALSE, "Failed to open dataFileLocal");
    // assert that we opened the file successfully
    //writeln("done");

    // ==== Create an array for each beer name (we have an assoc beername <==>
    // number of reviews)
    var BeerNameSpace: domain(string);
    var numReviewsLocal: [BeerNameSpace] int;

    // ==== For each block
    // We'll consider the owner of the firest replica to be the block owner
    for block in 0..#blockCount {

      // This might have to be changed back to the original once (if) we
      // switch to a cluster setup
      // ==== Get the host that the block resides on
      var owner_tmp = HDFS.chadoopGetHost(blockHosts, block: int(32), (block % fileInfo.mReplication): int(32)) + domainSuffix;
      var IDX = indexOf(".", owner_tmp, 1);
      var owner = owner_tmp.substring(1..IDX-1);

      // Past one (that apparently worked)
      //var owner = HDFS.chadoopGetHost(blockHosts, block: int(32), (block % fileInfo.mReplication): int(32)) + domainSuffix;
      //writeln("owner of block " + block + " is " + owner);

      // ==== if the locale we're on owns this block (i.e we own it)
      if (owner == here.name) {

        //writeln("locale " + here.id + " owns block " + block);

        // ==== Fetch the contents of the block
        var startByte = block * fileInfo.mBlockSize;
        var length = (fileInfo.mBlockSize + blockOverlap): int(32);
        if ((length + startByte) >= fileInfo.mSize) {
          length = (fileInfo.mSize - startByte): int(32);
        }

        // ==== Read in characters from the file
        //writeln("Positional read with startByte=" + startByte + " length=" + length);
        var s = HDFS.chadoopReadFilePositional(hdfsFS, dataFileLocal, startByte, length);
        // writeln("Read " + s.length + " characters from " + dataFile);

        // Read through the string and load it into the problem domain record
        var acc: BeerAdvocateRecord;
        var base = 1;

        // Parse data
        // Issue: far to domain-specific, brittle
        var numRead: int;

        do {
          // ==== Get the start of the review we want to parse
          var i = indexOf(recordStart, s, base);

          // ==== if we cant, say so and exit
          if (i == -1) {
            //writeln("Couldn't find start of record");
            break;
          }

          // Get the end of the review as well
          var j = indexOf(recordEnd, s, i + recordStart.length);

          if (j == -1) {
            // Find records at end of file
            if (block == blockCount - 1) {
              // Assume the current record extends to the end of file
              j = s.length;
              base = j;
            } else {
              //writeln("End of data reached");
              break;
            }
          } else {
            base = j;
          }

          // ==== Get the length (in characters) of the review in question
          var recordLength = (j - i + 1);
          // ==== Split what you read in, so now we have ss = review in question
          // Include the final field delimiter
          var ss = s.substring(i..j);
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

          if (j > fileInfo.mBlockSize) {
            //writeln("Found record end overhanging end of block at index " + j);
            break;
          }

        } while(true);
      }
    }

    writeln("numReviews on loc "); //+ loc.name + ": ");
    for  beerName in BeerNameSpace {
      writeln(beerName);
      writeln(numReviewsLocal[beerName]);
    }
    writeln("\n");

  } // on
} // for each locale


// ==== IndexOf(ss: string, s: string, start: int) -- Given a string s, we return the
// ==== index of the first occurence of the substring ss after start.
proc indexOf(substring: string, s: string, start: int): int {

  if (substring.length > s.length) {
    return -1;
  }

  for i in start..(s.length - substring.length + 1) {
    //writeln("i: " + i + ", s.length:" + s.length + ", substring.length: " + substring.length);
    var ss = s.substring(i..#substring.length);
    //writeln("ss: " + ss);
    if (ss == substring) {
      return i;
    }
  }

  return -1;
}

// ==== deserializeRecord(s: string) -- Parses a review into its internal
// ==== representation as a record
proc deserializeRecord(s: string): BeerAdvocateRecord {
  // FIXME: We don't need this whole record for the current purposes of this test. We
  // seem to be doing extra string work/using more memory and doing more work then we
  // need to necessarily (IMO)
  var res: BeerAdvocateRecord;
  var base: int = 1;
  (res.beer_name, base) = findFieldValue(s, "beer/name: ", base);
  (res.beer_beerId, base) = findFieldValueInt(s, "beer/beerId: ", base);
  (res.beer_brewerId, base) = findFieldValueInt(s, "beer/brewerId: ", base);
  (res.beer_ABV, base) = findFieldValueReal(s, "beer/ABV: ", base);
  (res.beer_style, base) = findFieldValue(s, "beer/style: ", base);
  (res.review_appearance, base) = findFieldValueReal(s, "review/appearance: ", base);
  (res.review_aroma, base) = findFieldValueReal(s, "review/aroma: ", base);
  (res.review_palate, base) = findFieldValueReal(s, "review/palate: ", base);
  (res.review_taste, base) = findFieldValueReal(s, "review/taste: ", base);
  (res.review_overall, base) = findFieldValueReal(s, "review/overall: ", base);
  (res.review_time, base) = findFieldValueInt(s, "review/time: ", base);
  (res.review_profileName, base) = findFieldValue(s, "review/profileName: ", base);
  return res;
}

// ==== findFieldValue(s: string, fieldStart: string, base: int) -- Returns a tuple
// ==== (field, value_of_field). i.e (beer, beerID)
proc findFieldValue(s: string, fieldStart: string, base: int): (string, int) {
  var i = indexOf(fieldStart, s, base);
  if (i == -1) {
    return ("", 1);
  }
  var j = indexOf("\n", s, i + fieldStart.length);
  if (j == -1) {
    return ("", 1);
  } else {
    return (s.substring(i + fieldStart.length..(j - 1)), j + 1);
  }
}
// ==== findFieldValueReal(s: string, fieldStart: string, base: int) -- Returns a
// tuple 
proc findFieldValueReal(s: string, fieldStart: string, base: int): (real, int) {
  var (a, b) = findFieldValue(s, fieldStart, base);
  if (a.length > 0) {
    return (a: real, b);
  } else {
    return (0.0, b);
  }
}
// ==== findFieldValueInt(s: string, fieldStart: string, base: int) -- Same here
proc findFieldValueInt(s: string, fieldStart: string, base: int): (int, int) {
  var (a, b) = findFieldValue(s, fieldStart, base);
  if (a.length > 0) {
    // TZ: ???? How is this legal?
    return (a: int, b);
  } else {
    return (0, b);
  }
}

// Tests, not really that important
proc testHdfs(hdfsFS: c_void_ptr) {

  var path = "/tmp/chadoop_test.txt";

  writeln("Opening " + path);
  var writeFile: c_void_ptr = HDFS.hdfsOpenFile(hdfsFS, path, O_WRONLY, 0, 0, 0);

  if (HDFS.IS_NULL(writeFile) == HDFS.IS_NULL_TRUE) {
    writeln("Failed to open file for writing: " + path);
    exit(1);
  } else {
    writeln("Successfully opened " + path);
  }		

  var buffer = "Hello from libhdfs";
  var written = HDFS.hdfsWrite(hdfsFS, writeFile, buffer, (buffer.length + 1): int(32));
  var flushed = HDFS.hdfsFlush(hdfsFS, writeFile);
  if (flushed != 0) {
    writeln("Failed to flush: " + path);
    exit(1);
  }

  HDFS.hdfsCloseFile(hdfsFS, writeFile);

  writeln("Reading from HDFS...");
  var readFile = HDFS.hdfsOpenFile(hdfsFS, path, O_RDONLY, 0, 0, 0);
  var read = HDFS.chadoopReadFilePositional(hdfsFS, readFile, 0, 1024);
  writeln(read);
  HDFS.chadoopFreeString(read);	
  read = HDFS.chadoopReadFilePositional(hdfsFS, readFile, 1, 1024);
  writeln(read);
  HDFS.chadoopFreeString(read);

  var hostBlocks = HDFS.hdfsGetHosts(hdfsFS, path, 0, 1000);
  var blockCount = HDFS.chadoopGetBlockCount(hostBlocks);
  writeln("blockCount: " + blockCount);
  var hostCount = HDFS.chadoopGetHostCount(hostBlocks, 0);
  writeln("hosts of block 0: " + hostCount);
  var host = HDFS.chadoopGetHost(hostBlocks, 0, 0);
  writeln("host 0 of block 0: " + host);
  HDFS.chadoopFree(hostBlocks);
  HDFS.hdfsCloseFile(hdfsFS, readFile);

  var blockSize = HDFS.hdfsGetDefaultBlockSize(hdfsFS);
  writeln("Default block size: " + blockSize);

  var info = HDFS.chadoopGetFileInfo(hdfsFS, path);
  writeln("size: " + info.mSize);
  writeln("replication: " + info.mReplication);
  writeln("block size: " + info.mBlockSize);
  writeln("permissions: " + info.mPermissions);
}

proc testIndexOf() {
  //writeln(indexOf("oo", "foo", 1));
  assert(indexOf("oo", "foo", 1) == 2);
  assert(indexOf("foo", "oo", 1) == -1);
  assert(indexOf("j", "foo", 1) == -1);
  assert(indexOf("fo", "foo", 2) == -1);
}
