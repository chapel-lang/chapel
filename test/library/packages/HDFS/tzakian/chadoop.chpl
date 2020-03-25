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
use Sys, HDFS, HDFStools;
// Second col is for running on a cluster
config const namenode = "default";//"sealcs01"; 
var connectNumber: c_int = 0; // 54310

//"localhost:9000"; 

config const dataFile = "/tmp/ba_sample.txt";
config const domainSuffix = "";
config const blockOverlap = 1024;
config const recordStart = "beer/name:";
config const recordEnd = "\n\n";
config param computeInParallel = false;

/* Problem domain */


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

writeln("localeHosts: " + localeHosts);
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
    write("Connecting to HDFS on host " + here.name + "...");
    var hdfsFS: c_void_ptr = HDFS.hdfsConnect(namenode.localize().c_str(), 
                                              connectNumber);
    writeln("done");
    var fileInfo = HDFS.chadoopGetFileInfo(hdfsFS, dataFile.localize().c_str());
    //      writeln("Info for file " + dataFile + ":");
    //      writeln(fileInfo);
    var blockHosts = HDFS.hdfsGetHosts(hdfsFS, dataFile.localize().c_str(), 0, fileInfo.mSize);
    // ==== Get the blocks
    var blockCount = HDFS.chadoopGetBlockCount(blockHosts);
    // END

    // ==== Connect to HDFS
    // ==== Open the file 
    writeln("Opening file " + dataFile + " on " + here.name + "...");
    var dataFileLocal = HDFS.hdfsOpenFile(hdfsFS, dataFile.localize().c_str(), 
                                          O_RDONLY, 0, 0, 0);
    assert(HDFS.IS_NULL(dataFileLocal) == HDFS.IS_NULL_FALSE, "Failed to open dataFileLocal");
    // assert that we opened the file successfully
    writeln("done");

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
      var owner_tmp = HDFS.chadoopGetHost(blockHosts, block: int(32), (block % fileInfo.mReplication): int(32)):string + domainSuffix;
      var IDX = indexOf(".", owner_tmp, 1);
      var owner = owner_tmp[1..IDX-1];

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
        var s = HDFS.chadoopReadFilePositional(hdfsFS, dataFileLocal, startByte, length):string;
        // writeln("Read " + s.size + " characters from " + dataFile);

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
            writeln("Couldn't find start of record");
            break;
          }

          // Get the end of the review as well
          var j = indexOf(recordEnd, s, i + recordStart.size);

          if (j == -1) {
            // Find records at end of file
            if (block == blockCount - 1) {
              // Assume the current record extends to the end of file
              j = s.size;
              base = j;
            } else {
              writeln("End of data reached");
              break;
            }
          } else {
            base = j;
          }

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

          if (j > fileInfo.mBlockSize) {
            writeln("Found record end overhanging end of block at index " + j);
            break;
          }

        } while(true);
      }
    }

    writeln("numReviews on loc " + loc.name + ": ");
    for  beerName in BeerNameSpace {
      write(beerName + "\t");
      writeln(numReviewsLocal[beerName]);
    }
    writeln("\n");

  } // on
} // for each locale


