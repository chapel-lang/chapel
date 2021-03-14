/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
   Iterators for distributed iteration over Hadoop Distributed Filesystem

   .. warning::

     This module is currently not working. See
     `#12627 <https://github.com/chapel-lang/chapel/issues/12627>`_
     to track progress.

   Iterators that can iterate over distributed data in an HDFS filesystem
   in a distributed manner. See :mod:`HDFS`.
 */
module HDFSiterator {

use HDFS, ReplicatedVar, RecordParser;

/* Iterate through an HDFS file (available in the default configured
   HDFS server) and yield records matching a regular expression.

   Serial and leader-follower versions of this iterator are available.

   :arg path: the path to the file within the HDFS server
   :arg rec: the type of the records to return
   :arg regexp: a regular expression with the same number of captures as
                the number of fields in the record type ``rec``

   */
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

// Lame use of a L/F, so it won't support zippering (so we would really like
// to have a standalone parallel iterator for this).
// Parallel IO and zippering will probably not play nice with each, due to not
// having a priori knowledge of how records map to positions in the file.
pragma "no doc"
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
        forall byteRange in workQueue[loc.id] with (ref globalFile) {
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

pragma "no doc"
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


} /* end of module */
