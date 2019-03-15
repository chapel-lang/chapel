/*
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

Support for Hadoop Distributed Filesystem

This module implements support for the
`Hadoop <http://hadoop.apache.org/>`_
`Distributed Filesystem <http://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-hdfs/HdfsUserGuide.html>`_ (HDFS).

Dependencies
------------

See the :ref:`auxIO-HDFS-deps` documentation for details on setting up and
enabling HDFS support in Chapel.

Using HDFS Support in Chapel
----------------------------

There are three ways provided to open HDFS files within Chapel.

Using an HDFS filesystem with open(url="hdfs://...")
****************************************************

.. code-block:: chapel

  // Open a file on HDFS connecting to the default HDFS instance
  var f = open(mode=iomode.r, url="hdfs://host:port/path");

  // Open up a reader and read from the file
  var reader = f.reader();

  // ...

  reader.close();

  f.close();


Explicitly Using Replicated HDFS Connections and Files
******************************************************

.. code-block:: chapel

  use HDFS;

  // Connect to HDFS via the default username (or whichever you want)
  //
  var hdfs = hdfsChapelConnect("default", 0);

  //
  // Create a file per locale
  //
  var gfl  = hdfs.hdfsOpen("/user/johnDoe/isThisAfile.txt", iomode.r);

  ...
  //
  // On any given locale, you can get the local file for the locale that
  // the task is currently running on via:
  //
  var fl = gfl.getLocal();

  // This file can be used as with a traditional file in Chapel, by
  // creating reader channels on it.

  // When you are done and want to close the files and disconnect from
  // HDFS, use:

  gfl.hdfsClose();
  hdfs.hdfsChapelDisconnect();

Explicitly Using Local HDFS Connections and Files
*************************************************

The HDFS module file also supports non-replicated values across
locales. So if you only wanted to connect to HDFS and open a file on
locale 1 you could do:

.. code-block:: chapel

  on Locales[1] {
    var hdfs = hdfs_chapel_connect("default", 0);
    var fl = hdfs.hdfs_chapel_open("/user/johnDoe/myFile.txt", iomode.cw);
    ...
    var read = fl.reader();
    ...
    fl.close();
    hdfs.hdfs_chapel_disconnect();
  }

The only stipulations are that you cannot open a file in both read and
write mode at the same time. (i.e iomode.r and iomode.cw are the only
modes that are supported, due to HDFS limitations).


HDFS Support Types and Functions
--------------------------------


 */
module HDFS {

use IO, SysBasic, SysError, ReplicatedVar, RecordParser;

pragma "no doc"
extern type qio_locale_map_ptr_t;     // array of locale to byte range mappings
pragma "no doc"
extern type char_ptr_ptr;             // char**

private extern const QIO_LOCALE_MAP_PTR_T_NULL: qio_locale_map_ptr_t;
private extern const hdfs_function_struct:qio_file_functions_t;
private extern const hdfs_function_struct_ptr:qio_file_functions_ptr_t;

pragma "no doc"
extern record hdfs_block_byte_map_t {
  var locale_id: int;
  var start_byte: int(64);
  var len: int(64);
}

// Connect to HDFS
private extern proc hdfs_connect(out fs: c_void_ptr, path: c_string, port: int): syserr;

// Disconnect from HDFS
private extern proc hdfs_disconnect(fs: c_void_ptr): syserr;

// Allocate an array for our locale mappings
private extern proc hdfs_alloc_array(n: int): char_ptr_ptr;

// Create a mapping locale_name -> locale_id (need this due to hdfs and since
// we cant pass strings inside extern records when multilocale)
private extern proc hdfs_create_locale_mapping(ref arr: char_ptr_ptr, num: int, loc_name: c_string);

// Return arr[i]
private extern proc hdfs_index_array(locs: qio_locale_map_ptr_t, i: int): hdfs_block_byte_map_t;

// Get block owners.
// Returns an array of hdfs_block_byte_map_t's
private extern proc hdfs_get_owners(f: qio_file_ptr_t, out locales: qio_locale_map_ptr_t, out num_blocks: c_int, arr: char_ptr_ptr, loc_nums:int): syserr;

// ********* For multilocale ************
/* Holds a file per locale */
record hdfsChapelFile {
  pragma "no doc"
  var files: [rcDomain] file;
}

/* Holds a connection to HDFS per locale */
record hdfsChapelFileSystem {
  pragma "no doc"
  var home: locale;
  pragma "no doc"
  var _internal_file: [rcDomain] c_void_ptr; // contains hdfsFS
}

// --------- Connecting/disconnecting ---------

/* Make a connection to HDFS for a single locale */
proc hdfsChapelConnect(path: c_string, port: int): c_void_ptr throws {
  var ret: c_void_ptr;
  var err = hdfs_connect(ret, path, port);
  if err then try ioerror(err, "Unable to connect to HDFS", path:string);
  return ret;
}

pragma "no doc"
proc hdfsChapelConnect(out error: syserr, path: c_string, port: int): c_void_ptr{
  compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
  var ret: c_void_ptr;
  try {
    ret = hdfsChapelConnect(path, port);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return ret;
}

/* Connect to HDFS and create a filesystem ptr per locale */
proc hdfsChapelConnect(path: string, port: int): hdfsChapelFileSystem throws {
  var ret: hdfsChapelFileSystem;
  try {
    forall loc in Locales with (ref ret) {
      on loc {
        var err: syserr;
        const tmpstr = path.localize();
        rcLocal(ret._internal_file) = hdfsChapelConnect(err, tmpstr.c_str(), port);
        if err then try ioerror(err, "Unable to connect to HDFS", tmpstr);
      }
    }
  }
  return ret;
}

/* Disconnect from the configured HDFS filesystem on each locale */
proc hdfsChapelFileSystem.hdfsChapelDisconnect() throws {
  try {
    forall loc in Locales {
      on loc {
        var err: syserr;
        err = hdfs_disconnect(rcLocal(this._internal_file));
        if err then try ioerror(err, "Unable to disconnect from HDFS");
      }
    }
  }
}

// ----- Opening/Closing ---------

/* Open a file on each locale */
proc hdfsChapelFileSystem.hdfsOpen(path:string, mode:iomode, hints:iohints=IOHINT_NONE, style:iostyle =
    defaultIOStyle()):hdfsChapelFile throws {
  var ret: hdfsChapelFile;
  try {
    forall loc in Locales with (ref ret) {
      on loc {
        var err:syserr = ENOERR;
        rcLocal(ret.files) = open(err, path, mode, hints, style, rcLocal(this._internal_file));
        if err then try ioerror(err, "in foreign open", path);
      }
    }
  }
  return ret;
}

pragma "no doc"
proc hdfsChapelFile.hdfsClose(out err: syserr) {
  err = qio_file_close(rcLocal(this.files)._file_internal);
}

/* Close a file opened with :proc:`hdfsChapelFileSystem.hdfsOpen` */
proc hdfsChapelFile.hdfsClose() throws {
  try {
    forall loc in Locales {
      on loc {
        var err: syserr = ENOERR;
        this.hdfsClose(err);
        if err then try ioerror(err, "Unable to close HDFS file");
      }
    }
  }
}

// ------------- General Utils ----------
/* Returns the file for the locale that we are on when we query */
pragma "no doc"
proc hdfsChapelFile.getLocal(): file {
  return rcLocal(this.files);
}

/* Convenience function. Does the same as file.reader except that we don't have
   to get our local file first */
pragma "no doc"
proc hdfsChapelFile.hdfsReader(param kind=iokind.dynamic, param locking=true, start:int(64) = 0, end:int(64) = max(int(64)), hints:iohints = IOHINT_NONE) {
  return rcLocal(this.files).reader(kind, locking, start, end, hints);
}

// ------------- End multilocale ---------------

pragma "no doc"
record hdfsChapelFile_local {
  var home: locale = here;
  var _internal_:qio_locale_map_ptr_t = QIO_LOCALE_MAP_PTR_T_NULL;
}

pragma "no doc"
record hdfsChapelFileSystem_local {
  var home: locale;
  var _internal_: c_void_ptr;
}

// this is used in hdfsChapelFileSystem_local.hdfs_chapel_open
proc open(path:string, mode:iomode, hints:iohints=IOHINT_NONE,
          style:iostyle = defaultIOStyle(), fs:c_void_ptr):file throws {
  var local_style = style;
  var ret:file;
  ret.home = here;
  var err = qio_file_open_access_usr(ret._file_internal, path.localize().c_str(),
                                     _modestring(mode).localize().c_str(),
                                     hints, local_style, fs,
                                     hdfs_function_struct_ptr);
  if err then try ioerror(err, "in foreign open", path);
  return ret;
}

pragma "no doc"
proc open(out error:syserr, path:string, mode:iomode, hints:iohints=IOHINT_NONE,
          style:iostyle = defaultIOStyle(), fs:c_void_ptr):file {
  compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
  var ret:file;
  ret.home = here;
  try {
    ret = open(path, mode, hints, style, fs);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return ret;
}

/* Open a file on an HDFS filesystem for a single locale */
proc hdfsChapelFileSystem_local.hdfs_chapel_open(
    path:string, mode:iomode, hints:iohints=IOHINT_NONE,
    style:iostyle = defaultIOStyle()):file throws {
  var ret: file;
  try {
    ret = open(path, mode, hints, style, this._internal_);
  } catch e: SystemError {
    try ioerror(e.err, "in foreign open", path);
  } catch {
    try ioerror(EINVAL:syserr, "in foreign open", path);
  }
  return ret;
}

pragma "no doc"
proc hdfsChapelFileSystem_local.hdfs_chapel_disconnect() throws {
  var err: syserr;
  on this.home {
    err = hdfs_disconnect(this._internal_);
  }
  if err then try ioerror(err, "Unable to disconnect from HDFS");
}

/* Connect to an HDFS filesystem on a single locale */
proc hdfs_chapel_connect(path:string, port: int): hdfsChapelFileSystem_local throws {
  var ret: hdfsChapelFileSystem_local;
  ret.home = here;
  var err = hdfs_connect(ret._internal_, path.localize().c_str(), port);
  if err then try ioerror(err, "Unable to connect to HDFS", path);
  return ret;
}

pragma "no doc"
proc hdfs_chapel_connect(out error:syserr, path:string, port: int): hdfsChapelFileSystem_local {
  compilerWarning("'out error: syserr' pattern has been deprecated, use 'throws' function instead");
  var ret: hdfsChapelFileSystem_local;
  try {
    ret = hdfs_chapel_connect(path, port);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return ret;
}

pragma "no doc"
proc getHosts(f: file) throws {
  var ret: hdfsChapelFile_local;
  var ret_num: c_int;
  var arr: char_ptr_ptr = hdfs_alloc_array(numLocales);
  for loc in Locales {
    hdfs_create_locale_mapping(arr, loc.id, loc.name.localize().c_str());
  }
  var err = hdfs_get_owners(f._file_internal, ret._internal_, ret_num, arr, numLocales);
  if err then try ioerror(err, "Unable to get owners for HDFS file");
  return (ret, ret_num);
}

pragma "no doc"
proc getLocaleBytes(g: hdfsChapelFile_local, i: int) {
  var ret = hdfs_index_array(g._internal_, i);
  return ret;
}


/*
   Iterators for distributed iteration over Hadoop Distributed Filesystem

   Iterators that can iterate over distributed data in an HDFS filesystem
   in a distributed manner. 
*/


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
