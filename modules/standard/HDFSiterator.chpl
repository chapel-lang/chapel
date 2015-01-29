use HDFS, UtilReplicatedVar, RecordParser;


iter HDFSiter(path: string, type rec, regex: string) {

  var hdfs = hdfs_chapel_connect("default", 0);
  var fl = hdfs.hdfs_chapel_open(path, iomode.r);


  var rr = fl.reader();
  var N  = new RecordReader(rec, rr, regex);
  // Make this migrate around to various locales based upon where the blocks reside
  // Reason we haven't done this is because we can't have a yield inside an on clause in a
  // serial iterator 
  for n in N.stream() {
    yield n;

  }

  fl.close();
  hdfs.hdfs_chapel_disconnect();
}

// Lame use of a L/F, so it won't support zippering (so we would really like to have
// a standalone parallel iterator for this).

// Parallel IO and zippering will probably not play nice with each, due to nt having
// a priori knowledge of how records map to positions in the file.

iter HDFSiter(param tag: iterKind, path: string, type rec, regex: string)
  where tag == iterKind.leader {

    // Represents (offset_in_file, length_of_block)
    type hdfsInfo = 2*int(64);

    // Create a work queue for each locale
    // Not actually a FIFO. (could be an unordered set)
    var workQueue: [LocaleSpace] domain(hdfsInfo);

    // ----- Create a file  and filesystem pointer per locale ----
    var hdfs = hdfsChapelConnect("default", 0);
    var globalFile   = hdfs.hdfsOpen(path, iomode.r);

    // -------- Get locales for blocks ------
    // Get the local file
    var localFile = globalFile.getLocal();

    // Get the mapping of host for blocks for this file along 
    // with how many blocks  there are
    var (hosts, numBlocksForFile) = getHosts(localFile);

    // Populate the work queue with work
    for block in 0..numBlocksForFile-1 {
      var h = getLocaleBytes(hosts, block);
      var r: hdfsInfo;
      r(1) = h.start_byte;
      r(2) = h.len;
      workQueue[h.locale_id] += r;
    }

    // NOTE: Second component on HDFs tuples are all the same except for the last
    // block. We could code around this to make it more compact (by specializing for
    // the last block).
    coforall loc in Locales {
      on loc {
        // Rip though the work in parallel on that locale
        forall byteRange in workQueue[loc.id] {
          var rr = globalFile.hdfsReader(start=byteRange(1));
          var N  = new RecordReader(rec, rr, regex);
          // Dont have a L/F for this so we yield serially. 
          for n in N.stream_num(byteRange(1), byteRange(2)) {
            yield n;
          }
        }
      }
    }

    globalFile.hdfsClose();
    hdfs.hdfsChapelDisconnect();
  }

iter HDFSiter(param tag: iterKind, path: string, type rec, regex: string, followThis)
  where tag == iterKind.follower {
    yield followThis;
  }


/* How hdfsOpen (and hdfsChapelConnect) works:

        +-----------------------+
        | hdfsOpen (globalFile) |
        +-----------------------+
                 /|\
                / | \
               /  |  \
              /   |   \
             /    |    \
     +--------+-------+------
     | File 1 | File2 | ...       File numLocales
     +--------+-------+------
   */
