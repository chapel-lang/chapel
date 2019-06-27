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

.. warning::

  This module is currently not working. See
  `#12627 <https://github.com/chapel-lang/chapel/issues/12627>`_
  to track progress.

*/

/*

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

use IO, SysBasic, SysError;

require "hdfs.h";
require "HDFSHelper/hdfsHelp.h";

extern type hdfsFS;   // opaque
extern type hdfsFile; // opaque
extern record hdfsFileInfo {
  var mSize:tOffset;
  var mBlockSize:tOffset;
}

extern type tSize = int(32);
extern type tOffset = int(64);
extern type tPort = uint(16);

private extern proc hdfsConnect(nn:c_string, port:tPort):hdfsFS;
private extern proc hdfsDisconnect(fs:hdfsFS):c_int;
private extern proc hdfsOpenFile(fs:hdfsFS, path:c_string, flags:c_int,
                                 bufferSize:c_int, replication:c_short,
                                 blockSize:tSize):hdfsFile;
private extern proc hdfsCloseFile(fs:hdfsFS, file:hdfsFile):c_int;
private extern proc chpl_hdfsIsFileNull(file:hdfsFile):bool;
private extern proc hdfsPread(fs:hdfsFS, file:hdfsFile, position:tOffset,
                              buffer:c_void_ptr, length:tSize):tSize;
private extern proc hdfsWrite(fs:hdfsFS, file:hdfsFile,
                              buffer:c_void_ptr, length:tSize):tSize;
private extern proc hdfsFlush(fs:hdfsFS, file:hdfsFile):c_int;
private extern proc hdfsGetPathInfo(fs:hdfsFS, path:c_string):c_ptr(hdfsFileInfo);
private extern proc hdfsFreeFileInfo(info:c_ptr(hdfsFileInfo), numEntries:c_int);

// QIO extern stuff
extern proc qio_strdup(s: c_string): c_string;
private extern proc qio_mkerror_errno():syserr;
private extern proc qio_channel_get_allocated_ptr_unlocked(ch:qio_channel_ptr_t, amt_requested:int(64), ref ptr_out:c_void_ptr, ref len_out:ssize_t, ref offset_out:int(64)):syserr;
private extern proc qio_channel_advance_available_end_unlocked(ch:qio_channel_ptr_t, len:ssize_t);
private extern proc qio_channel_get_write_behind_ptr_unlocked(ch:qio_channel_ptr_t, ref ptr_out:c_void_ptr, ref len_out:ssize_t, ref offset_out:int(64)):syserr;
private extern proc qio_channel_advance_write_behind_unlocked(ch:qio_channel_ptr_t, len:ssize_t);

param verbose = false;

proc connect(nameNode: string = "default", port:int=0) {
  return new hdfs(new unmanaged HDFSFileSystem(nameNode, port));
}

class HDFSFileSystem {
  var nameNode: string;
  var port: int;
  var hfs:hdfsFS;
  var refCount: atomic int;

  /* nameNode is the name node hostname, can be 'default'
     port is the port, can be '0' for default behavior */
  proc init(nameNode: string="default", port:int=0) {
    if verbose then
      writeln("hdfsConnect");

    this.nameNode = nameNode;
    this.port = port;
    this.hfs = hdfsConnect(this.nameNode.c_str(), this.port.safeCast(uint(16)));
    this.complete();
    refCount.write(1);
  }
  proc deinit() {
    if verbose then
      writeln("hdfsDisconnect");
    hdfsDisconnect(this.hfs);
  }
  proc retain() {
    refCount.add(1);
  }
  // should deallocate if the returned count is 0
  proc release() {
    var oldValue = refCount.fetchSub(1);
    return oldValue - 1;
  }

  proc open(path:string, mode:iomode,
            style:iostyle = defaultIOStyle(),
            in flags:c_int = 0, // default to based on mode
            bufferSize:c_int = 0,    // 0 -> use hdfs default value
            replication:c_short = 0, // 0 -> use hdfs default value
            blockSize:tSize = 0      // 0 -> use hdfs default value
           ) throws {

    if flags == 0 {
      // set flags based upon iomode
      select mode {
        when iomode.r {
          flags |= O_RDONLY;
        }
        when iomode.cw {
          flags |= O_CREAT | O_TRUNC | O_WRONLY;
        }
        when iomode.rw {
          flags |= O_RDWR;
        }
        when iomode.cwr {
          flags |= O_CREAT | O_TRUNC | O_RDWR;
        }
      }
    }

    if verbose then
      writeln("hdfsOpenFile");

    var hfile = hdfsOpenFile(this.hfs, path.localize().c_str(),
                             flags, bufferSize, replication, blockSize);

    if verbose then
      writeln("after hdfsOpenFile");

    if chpl_hdfsIsFileNull(hfile) {
      throw new owned SystemError(qio_mkerror_errno(), "in hdfsOpenFile");
    }

    // Create an HDFSFile and return the QIO file containing it
    // this initializer bumps the reference count to this
    var fl = new unmanaged HDFSFile(this:unmanaged, hfile, path);

    var ret: file;
    try {
      ret = openplugin(fl, mode, seekable=true, style);
    } catch e {
      fl.close();
      delete fl;
      throw e;
    }

    if verbose then
      writeln("after openplugin");

    return ret;
  }
}

record hdfs {
  forwarding var instance:unmanaged HDFSFileSystem;
  proc init(instance:unmanaged HDFSFileSystem) {
    this.instance = instance;
  }
  proc deinit() {
    var count = instance.release();
    if count == 0 then
      delete instance;
  }
}

class HDFSFile : QioPluginFile {
  var fs: unmanaged HDFSFileSystem;
  var hfile: hdfsFile;
  var path: string;

  proc init(fs:unmanaged HDFSFileSystem, hfile:hdfsFile, path:string) {
    if verbose then
      writeln("HDFSFile.init");

    this.fs = fs;
    this.fs.retain(); // bump reference count
    this.hfile = hfile;
    this.path = path;
  }

  override proc setupChannel(out pluginChannel:unmanaged QioPluginChannel,
                      start:int(64),
                      end:int(64),
                      qioChannelPtr:qio_channel_ptr_t):syserr {
    if verbose then
      writeln("HDFSFile.setupChannel");

    var hdfsch = new unmanaged HDFSChannel(this:unmanaged, qioChannelPtr);
    pluginChannel = hdfsch;
    return ENOERR;
  }

  override proc filelength(out length:int(64)):syserr {
    if verbose then
      writeln("HDFSFile.filelength path=", path);
    var fInfoPtr = hdfsGetPathInfo(fs.hfs, path.c_str());
    if fInfoPtr == nil {
      return EINVAL;
    }
    length = fInfoPtr.deref().mSize;
    hdfsFreeFileInfo(fInfoPtr, 1);
    if verbose then
      writeln("HDFSFile.filelength length=", length);
    return ENOERR;
  }
  override proc getpath(out path:c_string, out len:ssize_t):syserr {
    if verbose then
      writeln("HDFSFile.getpath path=", this.path);
    path = qio_strdup(this.path.c_str());
    len = this.path.size;
    if verbose then
      writeln("HDFSFile.getpath returning ", (path:string, len));
    return ENOERR;
  }

  override proc fsync():syserr {
    if verbose then
      writeln("HDFSFile.fsync");
    var rc = hdfsFlush(fs.hfs, hfile);
    if rc < 0 then
      return qio_mkerror_errno();
    return ENOERR;
  }
  override proc getChunk(out length:int(64)):syserr {
    if verbose then
      writeln("HDFSFile.getChunk");
    var fInfoPtr = hdfsGetPathInfo(fs.hfs, path.c_str());
    if fInfoPtr == nil then
      return EINVAL;
    length = fInfoPtr.deref().mBlockSize;
    hdfsFreeFileInfo(fInfoPtr, 1);
    return ENOERR;
  }
  override proc getLocalesForRegion(start:int(64), end:int(64), out
      localeNames:c_ptr(c_string), ref nLocales:int(64)):syserr {
    if verbose then
      writeln("HDFSFile.getLocalesForRegion");
    return ENOSYS;
  }

  override proc close():syserr {
    if verbose then
      writeln("hdfsCloseFile");

    var err:syserr = ENOERR;
    var rc = hdfsCloseFile(fs.hfs, hfile);
    if rc != 0 then
      err = qio_mkerror_errno();

    var count = fs.release();
    if count == 0 {
      delete fs;
      fs = nil;
    }
    return ENOERR;
  }
}

class HDFSChannel : QioPluginChannel {
  var file: unmanaged HDFSFile;
  var qio_ch:qio_channel_ptr_t;

  proc init(file: unmanaged HDFSFile, qio_ch:qio_channel_ptr_t) {
    if verbose then
      writeln("HDFSChannel.init");

    this.file = file;
    this.qio_ch = qio_ch;
  }

  override proc readAtLeast(amt:int(64)):syserr {
    if verbose then
      writeln("HDFSChannel.readAtLeast");

    if amt == 0 then
      return ENOERR;

    var err:syserr = ENOERR;

    var remaining = amt;
    while remaining > 0 {
      var ptr:c_void_ptr = c_nil;
      var len = 0;
      var offset = 0;
      err = qio_channel_get_allocated_ptr_unlocked(qio_ch, amt, ptr, len, offset);
      if err then
        return err;
      if ptr == nil || len == 0 then
        return EINVAL;

      if len >= max(int(32)) then
        len = max(int(32));

      // Now read len bytes into ptr
      var rc = hdfsPread(file.fs.hfs, file.hfile, offset, ptr, len:int(32));
      if rc == 0 {
        // end of file
        return EEOF;
      } else if rc < 0 {
        // error
        return qio_mkerror_errno();
      }
      // otherwise, rc is the number of bytes read.
      // advance the available region by the amount read
      qio_channel_advance_available_end_unlocked(qio_ch, rc);
      remaining -= rc:int(64);
    }

    return ENOERR;
  }
  override proc write(amt:int(64)):syserr {
    if verbose then
      writeln("HDFSChannel.write");

    var err:syserr = ENOERR;
    var remaining = amt;
    while remaining > 0 {
      var ptr:c_void_ptr = c_nil;
      var len = 0;
      var offset = 0;
      err = qio_channel_get_write_behind_ptr_unlocked(qio_ch, ptr, len, offset);
      if err then
        return err;
      if ptr == nil || len == 0 then
        return EINVAL;

      len = min(len, amt);
      if len >= max(int(32)) then
        len = max(int(32));

      if verbose then
        writeln("hdfsWrite");

      var rc = hdfsWrite(file.fs.hfs, file.hfile, ptr, len:int(32));
      if rc < 0 then
        return qio_mkerror_errno();

      qio_channel_advance_write_behind_unlocked(qio_ch, rc);
      remaining -= rc:int(64);
    }
    return ENOERR;
  }
  override proc close():syserr {
    return ENOERR;
  }
}

/*
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

private extern const hdfs_function_struct_ptr:qio_file_functions_ptr_t;
private extern proc hdfs_connect(out fs: c_void_ptr, path: c_string, port: int): syserr;
private extern proc hdfs_do_release(fs:c_void_ptr);

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
*/

} /* end of module */
