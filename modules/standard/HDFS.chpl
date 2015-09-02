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

Support for Hadoop Distributed Filesystem

This module implements support for the 
`Hadoop <http://hadoop.apache.org/>`_ 
`Distributed Filesystem <http://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-hdfs/HdfsUserGuide.html>`_ (HDFS).

Dependencies
------------

The HDFS functionality in Chapel is dependent upon both Hadoop and Java being
installed.  Your ``HADOOP_INSTALL``, ``JAVA_INSTALL`` and ``CLASSPATH``
environment variables must be set as described below in
:ref:`setting-up-hadoop`.  Without this it will not compile with HDFS, even if
the flags are set. As well, the HDFS functionality is also dependent upon the
``CHPL_AUXIO_INCLUDE`` and ``CHPL_AUXIO_LIBS`` environment variables being set
properly. For more information on how to set these properly, see 
doc/technotes/README.auxIO in a Chapel release.


Enabling HDFS Support
---------------------

Once you have ensured that Hadoop and Java are installed and have the
five variables above, defined, set the environment variable
CHPL_AUX_FILESYS to 'hdfs' to enable HDFS support:

.. code-block:: sh

  export CHPL_AUX_FILESYS=hdfs

Then, rebuild Chapel by executing 'make' from $CHPL_HOME.

.. code-block:: sh

  make

.. note::

  If HDFS support is not enabled (which is the default), all
  features described below will compile successfully but will result in
  an error at runtime such as: "No HDFS Support".


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


.. _setting-up-hadoop:

Setting up Hadoop 
-----------------

If you have a working installation of Hadoop already, you can skip
this section, other than to set up your CLASSPATH environment
variable.  This section is written so that people without sudo
permission can install and use HDFS.  If you do have sudo permissions,
you can usually install all of these via a package manager.

The general outline for these instructions is:

  1. Install and point to the jdk to provide code Chapel needs to
     compile against libhdfs (:ref:`Step 1 <setup-hadoop-1>`)
  2. Install Hadoop (:ref:`Step 2 <setup-hadoop-2>`)
  3. Set up Hadoop on (a) the local host or (b) a cluster of hosts
     (:ref:`Step 3 <setup-hadoop-3>`)
  4. Start up HDFS (:ref:`Step 4 <setup-hadoop-4>`)
  5. Stop HDFS when you're done (:ref:`Step 5 <setup-hadoop-5>`)
  6. Set up Chapel to run in distributed mode (:ref:`Step 6 <setup-hadoop-6>`)

First reflect your directory structure and version numbers (etc) in the
:ref:`sample .bashrc <setup-hadoop-bashrc>` and put it in your .bashrc (or
.bash_profile -- your choice) and source whichever one you put it into.

.. _setup-hadoop-1:

1. Make sure you have a SERVER edition of the jdk installed and
   point JAVA_INSTALL to it (see the same .bashrc below)

.. _setup-hadoop-2:

2. Install Hadoop

   * Download the latest version of Hadoop and unpack it

   * Now in the unpacked directory, open conf/hadoop-env.sh and edit:

     * set ``JAVA_INSTALL`` to be the part before ``bin/``... when you do:

        .. code-block:: sh

          which java

     * set ``HADOOP_CLASSPATH=$HADOOP_HOME/""*:$HADOOP_HOME/lib/""*:``

   * Now in conf/hdfs-site.xml put the replication number that you
     want for the field ``dfs.replication`` (this will set the
     replication of blocks of the files in HDFS)

   * Now set up passwordless ssh, if you haven't yet:

     .. code-block:: sh

       ssh-keygen -t dsa -P '' -f ~/.ssh/id_dsa
       cat ~/.ssh/id_dsa.pub >> ~/.ssh/authorized_keys

.. _setup-hadoop-3:

3. Set up Hadoop
   
   a. For the local host - See the
      `Hadoop website <http://hadoop.apache.org/docs/stable/single_node_setup.html>`_
      for good documentation on how to do this.

   b. For a cluster of hosts. If you want to run Hadoop over a cluster, there
      are good tutorials online. Although it is usually as easy as making
      edits to the following files in ``$HADOOP_HOME/conf``:

      * adding the name of the nodes to ``slaves``
      * putting what you want to be the namenode in ``masters``
      * putting the master node in ``core-site.xml`` and ``mapred-site.xml``
      * running:

        .. code-block:: sh

         hadoop-daemon.sh start datanode
         hadoop-daemon.sh start tasktracker

      After this go to your datanode site and you should see a new
      datanode.

      A good online tutorial for this as well can be found here:
      http://hadoop.apache.org/docs/stable/cluster_setup.html

.. _setup-hadoop-4:

4. Start HDFS

   * Now all we need to do is format the namenode and start things up:

     .. code-block:: sh

       hadoop namenode -format
       start-all.sh  # (This will start hdfs and the tasktracker/jobtracker)

   * In general, hadoop has the same type of commands as bash,
     usually in the form:

     .. code-block:: sh

         hadoop dfs -<command> <regular args to that command>

   * At this point, you can compile and run Chapel programs using HDFS

   * You can check the status of Hadoop via http, for example on a local
     host (e.g., for :ref:`3a above <setup-hadoop-3>`), using:

       *  http://localhost:50070/
       *  http://localhost:50030/

     For cluster mode (:ref:`3b <setup-hadoop-3>`), you'll use the name of the
     master host in the URL and its port (see the web for details).

.. _setup-hadoop-5:

5. Shut things down:

   .. code-block:: sh

     stop-all.sh   # (This will stop hdfs and mapreduce)

.. _setup-hadoop-6:

6. Set up Chapel to run in distributed mode:

   * You'll need to set up your Chapel environment to target multiple
     locales in the standard way (see README.multilocale and the
     "Settings to run Chapel on multiple nodes" section of the
     .bashrc below).

   * After this you should be able to run Chapel code with HDFS over
     a cluster of computers the same way as you normally would.
    

.. _setup-hadoop-bashrc:

Here is a sample .bashrc for using Hadoop within Chapel:


.. code-block:: sh

  #
  # For Hadoop
  #
  export HADOOP_INSTALL=<Place where you have Hadoop installed>
  export HADOOP_HOME=$HADOOP_INSTALL
  export HADOOP_VERSION=<Your Hadoop version number>
  #
  # Note that the following environment variables might contain more paths than
  # those listed below if you have more than one IO system enabled. These are all
  # that you will need in order to use HDFS (only)
  #
  export CHPL_AUXIO_INCLUDE="-I$JAVA_INSTALL/include -I$JAVA_INSTALL/include/linux  -I$HADOOP_INSTALL/src/c++/libhdfs"
  export CHPL_AUXIO_LIBS="-L$JAVA_INSTALL/jre/lib/amd64/server -L$HADOOP_INSTALL/c++/Linux-amd64-64/lib"
  
  #
  # So we can run things such as start-all.sh etc. from anywhere and
  # don't need to be in $HADOOP_INSTALL
  #
  export PATH=$PATH:$HADOOP_INSTALL/bin
  
  #
  # Point to the JDK installation
  #
  export JAVA_INSTALL=<Place where you have the jdk installed>
  
  #
  # Add Hadoop directories to the Java class path
  #
  export CLASSPATH=$CLASSPATH:$HADOOP_HOME/""*:$HADOOP_HOME/lib/""*:$HADOOP_HOME/conf/""*:$(hadoop classpath):
  
  #
  # So we don't have to "install" these things
  #
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HADOOP_HOME/c++/Linux-amd64-64/lib:$HADOOP_HOME/src/c++/libhdfs:$JAVA_INSTALL/jre/lib/amd64/server:$JAVA_INSTALL:$HADOOP_HOME/lib:$JAVA_INSTALL/jre/lib/amd64:$CLASSPATH
  
  #
  # Settings to run Chapel on multiple nodes
  #
  export GASNET_SPAWNFN=S
  export SSH_SERVERS=<the names of the computers in your cluster>
  export SSH_CMD=ssh
  export SSH_OPTIONS=-x
  export GASNET_ROUTE_OUTPUT=0

HDFS Support Types and Functions
--------------------------------


 */
module HDFS {

use IO, SysBasic, Error, UtilReplicatedVar;

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
proc hdfsChapelConnect(out error: syserr, path: c_string, port: int): c_void_ptr{
  var ret: c_void_ptr;
  error = hdfs_connect(ret, path, port);
  return ret;
}

/* Connect to HDFS and create a filesystem ptr per locale */
proc hdfsChapelConnect(path: string, port: int): hdfsChapelFileSystem {
  var ret: hdfsChapelFileSystem;
  forall loc in Locales {
    on loc {
      var err: syserr;
      const tmpstr = path.localize();
      rcLocal(ret._internal_file) = hdfsChapelConnect(err, tmpstr.c_str(), port);
      if err then ioerror(err, "Unable to connect to HDFS", tmpstr);
    }
  }
  return ret;
}

/* Diconnect from the configured HDFS filesystem on each locale */
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

/* Open a file on each locale */
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

pragma "no doc"
proc hdfsChapelFile.hdfsClose(out err: syserr) {
  err = qio_file_close(rcLocal(this.files)._file_internal);
}

/* Close a file opened with :proc:`hdfsChapelFileSystem.hdfsOpen` */
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
/* Returns the file for the locale that we are on when we query */
pragma "no doc"
proc hdfsChapelFile.getLocal(): file {
  return rcLocal(this.files);
}

/* Convenience funtion. Does the same as file.reader except that we dont have
   to get our local file first */
pragma "no doc"
proc hdfsChapelFile.hdfsReader(param kind=iokind.dynamic, param locking=true, start:int(64) = 0, end:int(64) = max(int(64)), hints:iohints = IOHINT_NONE) {
  return rcLocal(this.files).reader(kind, locking, start, end, hints);
}

// ------------- End mulitlocale ---------------

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

pragma "no doc"
// this is used in hdfsChapelFileSystem_local.hdfs_chapel_open
proc open(out error:syserr, path:string, mode:iomode, hints:iohints=IOHINT_NONE,
    style:iostyle = defaultIOStyle(), fs:c_void_ptr):file {
  var local_style = style;
  var ret:file;
  ret.home = here;
  error = qio_file_open_access_usr(ret._file_internal, path, _modestring(mode),
      hints, local_style, fs, hdfs_function_struct_ptr); 
  return ret;
}

/* Open a file on an HDFS filesystem for a single locale */
proc hdfsChapelFileSystem_local.hdfs_chapel_open(path:string, mode:iomode, hints:iohints=IOHINT_NONE, style:iostyle = defaultIOStyle()):file {
  var err:syserr = ENOERR;
  var ret = open(err, path, mode, hints, style, this._internal_);
  if err then ioerror(err, "in foreign open", path);
  return ret;
}

pragma "no doc"
proc hdfsChapelFileSystem_local.hdfs_chapel_disconnect() {
  on this.home {
    var err: syserr;
    err = hdfs_disconnect(this._internal_);
    if err then ioerror(err, "Unable to disconnect from HDFS");
  }
}

/* Connect to an HDFS filesystem on a single locale */
proc hdfs_chapel_connect(path:string, port: int): hdfsChapelFileSystem_local{

  var err: syserr;
  var ret = hdfs_chapel_connect(err, path, port);
  if err then ioerror(err, "Unable to connect to HDFS", path);
  return ret;
}

pragma "no doc"
proc hdfs_chapel_connect(out error:syserr, path:string, port: int): hdfsChapelFileSystem_local{
  var ret:hdfsChapelFileSystem_local;
  ret.home = here;
  error = hdfs_connect(ret._internal_, path.localize().c_str(), port);
  return ret;
}

pragma "no doc"
proc getHosts(f: file) {
  var ret: hdfsChapelFile_local;
  var ret_num: c_int;
  var arr: char_ptr_ptr = hdfs_alloc_array(numLocales);
  for loc in Locales {
    hdfs_create_locale_mapping(arr, loc.id, loc.name.localize().c_str());
  }
  var err = hdfs_get_owners(f._file_internal, ret._internal_, ret_num, arr, numLocales);
  if err then ioerror(err, "Unable to get owners for HDFS file");
  return (ret, ret_num);
}

pragma "no doc"
proc getLocaleBytes(g: hdfsChapelFile_local, i: int) {
  var ret = hdfs_index_array(g._internal_, i);
  return ret;
}

} /* end of module */
