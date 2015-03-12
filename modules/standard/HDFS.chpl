/*
 * Copyright 2004-2015 Cray Inc.
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
   HDFS module
 */
module HDFS {

use IO, SysBasic, Error, UtilReplicatedVar;

extern type qio_locale_map_ptr_t;     // array of locale to byte range mappings
extern type char_ptr_ptr;             // char**

extern const QIO_LOCALE_MAP_PTR_T_NULL: qio_locale_map_ptr_t;
extern const hdfs_function_struct:qio_file_functions_t;
extern const hdfs_function_struct_ptr:qio_file_functions_ptr_t;

extern record hdfs_block_byte_map_t {
  var locale_id: int; 
  var start_byte: int(64); 
  var len: int(64);
}

// Connect to HDFS
extern proc hdfs_connect(out fs: c_void_ptr, path: c_string, port: int): syserr; 

// Disconnect from HDFS
extern proc hdfs_disconnect(fs: c_void_ptr): syserr;

// Allocate an array for our locale mappings
extern proc hdfs_alloc_array(n: int): char_ptr_ptr;

// Create a mapping locale_name -> locale_id (need this due to hdfs and since we cant
// pass strings inside extern records when multilocale)
extern proc hdfs_create_locale_mapping(ref arr: char_ptr_ptr, num: int, loc_name: c_string);

// Return arr[i]
extern proc hdfs_index_array(locs: qio_locale_map_ptr_t, i: int): hdfs_block_byte_map_t;

// Get block owners.
// Returns an array of hdfs_block_byte_map_t's
extern proc hdfs_get_owners(f: qio_file_ptr_t, out locales: qio_locale_map_ptr_t, out num_blocks: c_int, arr: char_ptr_ptr, loc_nums:int): syserr;

// ********* For multilocale ************
// Holds a file per locale
record hdfsChapelFile {
  var files: [rcDomain] file;
}

// Holds a configured HDFS filesystem per locale
record hdfsChapelFileSystem {
  var home: locale;
  var _internal_file: [rcDomain] c_void_ptr; // contains hdfsFS
}

// --------- Connecting/disconnecting ---------

// Connect to HDFS
proc hdfsChapelConnect(out error: syserr, path: c_string, port: int): c_void_ptr{
  var ret: c_void_ptr;
  error = hdfs_connect(ret, path, port);
  return ret;
}

// Connect to HDFS and create a filesystem ptr per locale
proc hdfsChapelConnect(path: string, port: int): hdfsChapelFileSystem {
  var ret: hdfsChapelFileSystem;
  forall loc in Locales {
    on loc {
      var err: syserr;
      // XXX: HACK: this accessing of locale.id copies the string onto this locale...
      // If you get rid of this IT WILL BREAK. (at least as strings currently stand)
      var hack = path.locale.id;
      const tmpstr = path;
      rcLocal(ret._internal_file) = hdfsChapelConnect(err, tmpstr.c_str(), port);
      if err then ioerror(err, "Unable to connect to HDFS", tmpstr);
    }
  }
  return ret;
}

// Diconnect from the configured HDFS filesystem on each locale
proc hdfsChapelFileSystem.hdfsChapelDisconnect() {
  forall loc in Locales {
    on loc {
      var err: syserr;
      err = hdfs_disconnect(rcLocal(this._internal_file));
      if err then ioerror(err, "Unable to disconnect from HDFS");
    }
  }
}

// ----- Opening/Closing ---------

// Open a file on each locale
proc hdfsChapelFileSystem.hdfsOpen(path:string, mode:iomode, hints:iohints=IOHINT_NONE, style:iostyle =
    defaultIOStyle()):hdfsChapelFile {
  var err:syserr = ENOERR;
  var ret: hdfsChapelFile;
  forall loc in Locales {
    on loc {
      rcLocal(ret.files) = open(err, path, mode, hints, style, rcLocal(this._internal_file));
      if err then ioerror(err, "in foreign open", path);
    }
  }
  return ret;
}

proc hdfsChapelFile.hdfsClose(out err: syserr) {
  err = qio_file_close(rcLocal(this.files)._file_internal);
}

// Close each file on each locale that we created
proc hdfsChapelFile.hdfsClose() {
  forall loc in Locales {
    on loc {
      var err: syserr = ENOERR;
      this.hdfsClose(err);
      if err then ioerror(err, "Unable to close HDFS file");
    }
  }
}

// ------------- General Utils ----------
// Returns the file for the locale that we are on when we query
proc hdfsChapelFile.getLocal(): file {
  return rcLocal(this.files);
}

// Convenience funtion. Does the same as file.reader except that we dont have to get
// our local file first
proc hdfsChapelFile.hdfsReader(param kind=iokind.dynamic, param locking=true, start:int(64) = 0, end:int(64) = max(int(64)), hints:iohints = IOHINT_NONE) {
  return rcLocal(this.files).reader(kind, locking, start, end, hints);
}

// ------------- End mulitlocale ---------------

record hdfsChapelFile_local {
  var home: locale = here;
  var _internal_:qio_locale_map_ptr_t = QIO_LOCALE_MAP_PTR_T_NULL;
}

record hdfsChapelFileSystem_local {
  var home: locale;
  var _internal_: c_void_ptr;
}

proc open(out error:syserr, path:string, mode:iomode, hints:iohints=IOHINT_NONE,
    style:iostyle = defaultIOStyle(), fs:c_void_ptr):file {
  var local_style = style;
  var ret:file;
  ret.home = here;
  error = qio_file_open_access_usr(ret._file_internal, path, _modestring(mode),
      hints, local_style, fs, hdfs_function_struct_ptr); 
  return ret;
}

proc hdfsChapelFileSystem_local.hdfs_chapel_open(path:string, mode:iomode, hints:iohints=IOHINT_NONE, style:iostyle = defaultIOStyle()):file {
  var err:syserr = ENOERR;
  var ret = open(err, path, mode, hints, style, this._internal_);
  if err then ioerror(err, "in foreign open", path);
  return ret;
}

proc hdfsChapelFileSystem_local.hdfs_chapel_disconnect() {
  on this.home {
    var err: syserr;
    err = hdfs_disconnect(this._internal_);
    if err then ioerror(err, "Unable to disconnect from HDFS");
  }
}

proc hdfs_chapel_connect(path:string, port: int): hdfsChapelFileSystem_local{

  var err: syserr;
  var ret = hdfs_chapel_connect(err, path, port);
  if err then ioerror(err, "Unable to connect to HDFS", path);
  return ret;
}

proc hdfs_chapel_connect(out error:syserr, path:string, port: int): hdfsChapelFileSystem_local{
  var ret:hdfsChapelFileSystem_local;
  ret.home = here;
  error = hdfs_connect(ret._internal_, path.c_str(), port);
  return ret;
}

proc getHosts(f: file) {
  var ret: hdfsChapelFile_local;
  var ret_num: c_int;
  var arr: char_ptr_ptr = hdfs_alloc_array(numLocales);
  for loc in Locales {
    hdfs_create_locale_mapping(arr, loc.id, loc.name.c_str());
  }
  var err = hdfs_get_owners(f._file_internal, ret._internal_, ret_num, arr, numLocales);
  if err then ioerror(err, "Unable to get owners for HDFS file");
  return (ret, ret_num);
}

proc getLocaleBytes(g: hdfsChapelFile_local, i: int) {
  var ret = hdfs_index_array(g._internal_, i);
  return ret;
}

} /* end of module */
