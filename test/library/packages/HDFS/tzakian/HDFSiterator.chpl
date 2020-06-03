// HDFS iteration (AKA "map") module

/* ====================== TODO's ========================
 * 
 * Need to optimize this ALOT more....
 
 * Need to look at host selection, apparently what I thought was happening actually
   wasnt... so we need to look at this as well

 * Need to make sure that it is seeking properly in the file when we have blocks
   of one file spread across many hosts.
   -- See bottom of file for diagram

 * How to deal with breaks across blocks: For instance if we had:
   
   "... the brown fox jumped over the brown cow that was jumping over the moon"
   
   and we had a block structure:
   
   [... the brown fox jumped over the bro](b1) [wn cow that was jumping over the moon] (b2)

   Then wordCount on the first phrase will be 14 whereas for the second, it will be 15

  * =============== PROGRAMMER ASSUMPTIONS (at least for now) ============================
  
  * The programmer is responsible for making sure that they take care of block splits
     -- must know what a record end/start is and code from there

  * The programmer is responsible for collecting the values from the map (for now I'm
    using an array)

  * The programmer is responsible for writing a reducer for the reduce that they wish
    to happen (string based, sum etc)

 */

use Sys, 
    HDFS,
    HDFStools, 
    ReplicatedVar,
    BlockDist;

// ============== Serial iterator ========================
iter HDFSmap(dataFile: string, namenode: string = "default", port: int(32) = 0) {


  // use const instead of var -- better optimizations this way
  const hdfsFS: c_void_ptr = HDFS.hdfsConnect(namenode.localize().c_str(), port);
  const fileInfo = HDFS.chadoopGetFileInfo(hdfsFS, dataFile.localize().c_str());
  const blockHosts = HDFS.hdfsGetHosts(hdfsFS, dataFile.localize().c_str(), 0, fileInfo.mSize); // incr 0?
  const blockCount = HDFS.chadoopGetBlockCount(blockHosts);
  const dataFileLocal = HDFS.hdfsOpenFile(hdfsFS, dataFile.localize().c_str(), O_RDONLY, 0, 0, 0);
  assert(HDFS.IS_NULL(dataFileLocal) == HDFS.IS_NULL_FALSE, "Failed to open dataFileLocal");
  var length = (fileInfo.mBlockSize): int(32); // LOOK

  var Blocks: [LocaleSpace] domain(int);
  // Setup a mapping loc --> block

  for i in 0..#blockCount {

    var owner_tmp = HDFS.chadoopGetHost(blockHosts, i: int(32), (i % fileInfo.mReplication): int(32)):string;
    var IDX = indexOf(".", owner_tmp, 1);
    var owner = owner_tmp[1..IDX-1];

    for loc in Locales {
      if (loc.name == owner)
        then Blocks[loc.id] += i;
    }
  }

  for loc in Locales { // add on clause
    for block in Blocks[loc.id] {
      var startByte = block * fileInfo.mBlockSize;
      var length_tmp = length;
      if ((length + startByte) >= fileInfo.mSize) {
        length_tmp = (fileInfo.mSize - startByte): int(32); // or get rid of -1
      }
      var s = HDFS.chadoopReadFilePositional(hdfsFS, dataFileLocal, startByte, length_tmp);
      yield (s, block); // owner is the locale that "owns" that block
    }
  }
  hdfsCloseFile(hdfsFS, dataFileLocal);
}

// ======= Leader-follower iterator that should implement the above in parallel ====
iter HDFSmap(param tag: iterKind, dataFile: string, namenode: string = "default",
    port: int(32) = 0)
  where tag == iterKind.leader {

    // ================== Globals and replication domains ==================
    var onceOnly: atomic bool; 
    onceOnly.write(false);

    var blockOwners: domain(string); 

    // populate here so we dont have to dynamically resize
    for loc in Locales do
      blockOwners.add(loc.name);

    var Blockies: [blockOwners] domain(int);

    // Setup replication across our locales
    const hdfsFS_PL:        [rcDomain] c_void_ptr;
    const fileInfo_PL:      [rcDomain] chadoopFileInfo;
    const dataFileLocal_PL: [rcDomain] c_void_ptr; 
    const blockHosts_PL:    [rcDomain] c_void_ptr;
    const blockCount_PL:    [rcDomain] c_int;
    const length_PL:        [rcDomain] int(32);
    // ====================== END ==========================================

    coforall loc in Locales { 
      on loc {

        //======================== File connection =========================
        var hdfsFS: c_void_ptr = HDFS.hdfsConnect(namenode.localize().c_str(), port);
        assert(HDFS.IS_NULL(hdfsFS) == HDFS.IS_NULL_FALSE, "Failed to connect to HDFS");

        var fileInfo      = HDFS.chadoopGetFileInfo(hdfsFS, dataFile.localize().c_str());
        var dataFileLocal = HDFS.hdfsOpenFile(hdfsFS, dataFile.localize().c_str(), O_RDONLY, 0, 0, 0);
        assert(HDFS.IS_NULL(dataFileLocal) == HDFS.IS_NULL_FALSE, "Failed to open dataFileLocal on loc ", here.id);
        // =================================== END =========================

        // ========== Native (Chapel) constants used throughout ============
        // These should all be covered under the PGAS model -- right?? 
        var length = (fileInfo.mBlockSize): int(32);
        // Our length is going to be one less then block size since we want to 
        // read up to the point that the next read takes over (i.e We always read 0-(n-1))
        // =================================== END =========================

        // ================ Replication to Locales =========================
        // Copy over our stuff to all of our locales
        rcLocal(hdfsFS_PL)        = hdfsFS;
        rcLocal(fileInfo_PL)      = fileInfo;
        rcLocal(dataFileLocal_PL) = dataFileLocal;
        rcLocal(length_PL)        = length;
        // =================================== END =========================
      }
    }

    // ========================== File-blocks ========================== 
    // Get block info
    if (!onceOnly.testAndSet()) { // we might be able to get rid of the atomic here
      // Setup a mapping loc --> {block1, block2, ...}
      var fileInfo = rcLocal(fileInfo_PL);
      var hdfsFS   = rcLocal(hdfsFS_PL);
      var blockHosts = HDFS.hdfsGetHosts(hdfsFS, dataFile.localize().c_str(), 0, fileInfo.mSize); // Investigate
      var blockCount: int = HDFS.chadoopGetBlockCount(blockHosts);

      const Space = {0..blockCount-1};
      const Bspace = Space dmapped Block(boundingBox=Space);
      var Biter: [Bspace] int;

      forall (j, i) in zip (Biter, 0..) {
        var fileInfo = rcLocal(fileInfo_PL);
        var hdfsFS   = rcLocal(hdfsFS_PL);
        var blockHosts = HDFS.hdfsGetHosts(hdfsFS, dataFile.localize().c_str(), 0, fileInfo.mSize); // Investigate

        //var owner_tmp = HDFS.chadoopGetHost(blockHosts, i: int(32), here.name + domainSuffix, (i % fileInfo.mReplication): int(32));
        var owner_tmp = HDFS.chadoopGetHost(blockHosts, i: int(32), (i % fileInfo.mReplication): int(32)):string;
        var IDX = indexOf(".", owner_tmp, 1);
        var owner = owner_tmp[1..IDX-1];
        Blockies(owner) += i;
        //printBlockHosts_C(blockHosts, here.name);
      }
    }
    // =================================== END =========================

    // ======= Pop off all our work on each locale work-queue on that locale ======
    //
    coforall loc in Locales { 
      on loc {

        // Get replicated values
        var fileInfo_LOCAL = rcLocal(fileInfo_PL);
        var hdfsFS_LOCAL = rcLocal(hdfsFS_PL);
        var dataFileLocal_LOCAL = rcLocal(dataFileLocal_PL);
        var length_LOCAL = rcLocal(length_PL);

        // Run through our "work queue"
        forall block in Blockies[loc.name] { // look at sorted order
          var length_tmp = length_LOCAL;
          // -- see bottom of page
          var startByte = block * fileInfo_LOCAL.mBlockSize; // GOOD
          if ((length_LOCAL + startByte) >= fileInfo_LOCAL.mSize) { 
            // we've reached EOF so we dont take off that byte (since we want to read
            // till the end of the file)
            // change to length_tmp ... makes safe in case we reach this case first
            length_tmp = (fileInfo_LOCAL.mSize - startByte): int(32); 
            // So go to the end of the file and subtract off to where weve read already
            // to get where we should start reading from
          } //else length_tmp = length_LOCAL

          var s = HDFS.chadoopReadFilePositional(hdfsFS_LOCAL, dataFileLocal_LOCAL, startByte, length_tmp);
          yield (s, block); // owner is the locale that "owns" that block
        }
      }
    }
    // ============= Close our file on each locale ==============
    for loc in Locales { 
      on loc { 
        var hdfsFS_LOCAL = rcLocal(hdfsFS_PL);
        var dataFileLocal_LOCAL = rcLocal(dataFileLocal_PL);
        hdfsCloseFile(hdfsFS_LOCAL, dataFileLocal_LOCAL);
      }
    }
  }


// ======== Follower iterator =========================
iter HDFSmap(param tag: iterKind, dataFile: string, namenode: string = "default",
    port: int(32) = 0, followThis)
  where tag == iterKind.follower {
    yield followThis;
  }


/* Seeking reasoning/diagram:

   +----------------------+ <== 0 = block# * length := startByte
   |                      |  \
   |                      |   \
   |                      |    |
   |                      |    |
   |      Block 0         |    |
   |                      |    |
   |                      |    | 1024 bytes = length + 1
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |   /
   |                      | <============================= 0 + 1023 (bytes) = startByte + length - 1
   +----------------------+ <== 1024 = block# * length = startByte
   |                      |  \
   |                      |   \
   |                      |    |
   |                      |    |
   |                      |    |
   |      Block 1         |    |
   |                      |    | 1024 bytes = length
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |   /
   |                      | <============================= 1024 + 1023 (bytes) = startByte + length - 1
   +----------------------+ <== 2048 = block# * length = startByte
   |                      |  \
   |                      |   \
   |                      |    |
   |                      |    |
   |                      |    |
   |      Block 2         |    |
   |                      |    | 1024 bytes = length
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |    |
   |                      |   /
   |                      | <============================= 2048 + 1023 (bytes) = startByte + length - 1
   +----------------------+ <== 3072 = block# * length = startByte
   .
   .
   .
   .

 * The statement: fileLength - startByte = LengthToEOF
 -- Always holds

 */









