.. default-domain:: chpl

.. module:: HDFS
   :synopsis: This module implements support for the

HDFS
====
**Usage**

.. code-block:: chapel

   use HDFS;



This module implements support for the
`Hadoop <http://hadoop.apache.org/>`_
`Distributed Filesystem <http://hadoop.apache.org/docs/current/hadoop-project-dist/hadoop-hdfs/HdfsUserGuide.html>`_ (HDFS).

.. note::

  HDFS support in Chapel currently requires the use of ``CHPL_TASKS=fifo``.
  There is a compatibility problem with qthreads.

Using HDFS Support in Chapel
----------------------------

To open an HDFS file in Chapel, first create an :class:`HDFSFileSystem` by
connecting to an HDFS name node.

.. code-block:: chapel

  import HDFS;

  fs = HDFS.connect(); // can pass a nameNode host and port here,
                       // otherwise uses HDFS default settings.

The filesystem connection will be closed when `fs` and any files
it refers to go out of scope.

Once you have a :record:`hdfs`, you can open files within that
filesystem using :proc:`HDFSFileSystem.open` and perform I/O on them using
the usual functionality in the :mod:`IO` module:

.. code-block:: chapel

  var f = fs.open("/tmp/testfile.txt", iomode.cw);
  var writer = f.writer();
  writer.writeln("This is a test");
  writer.close();
  f.close();

.. note::

  Please note that ``iomode.cwr`` and ``iomode.rw`` are not supported with HDFS
  files due to limitations in HDFS itself. ``iomode.r`` and ``iomode.cw`` are
  the only modes supported with HDFS.

Dependencies
------------

Please refer to the Hadoop and HDFS documentation for instructions on setting up
HDFS.

Once you have a working HDFS, it's a good idea to test your HDFS installation
with a C program before proceeding with Chapel HDFS support. Try compiling the
below C program:

.. code-block:: c

  // hdfs-test.c

  #include <hdfs.h>

  #include <string.h>
  #include <stdio.h>
  #include <stdlib.h>

  int main(int argc, char **argv) {

      hdfsFS fs = hdfsConnect("default", 0);
      const char* writePath = "/tmp/testfile.txt";
      hdfsFile writeFile = hdfsOpenFile(fs, writePath, O_WRONLY|O_CREAT, 0, 0, 0);
      if(!writeFile) {
            fprintf(stderr, "Failed to open %s for writing!\n", writePath);
            exit(-1);
      }
      char* buffer = "Hello, World!";
      tSize num_written_bytes = hdfsWrite(fs, writeFile, (void*)buffer, strlen(buffer)+1);
      if (hdfsFlush(fs, writeFile)) {
             fprintf(stderr, "Failed to 'flush' %s\n", writePath);
            exit(-1);
      }
     hdfsCloseFile(fs, writeFile);
  }

This program will probably not compile without some special environment
variables set.  The following commands worked for us to compile this program,
but you will almost certainly need different settings depending on your HDFS
installation.

.. code-block:: bash

  export JAVA_HOME=/usr/lib/jvm/default-java/lib
  export HADOOP_HOME=/usr/local/hadoop/
  gcc hdfs-test.c -I$HADOOP_HOME/include -L$HADOOP_HOME/lib/native -lhdfs
  export CLASSPATH=`$HADOOP_HOME/bin/hadoop classpath --glob`
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$HADOOP_HOME/lib/native:$JAVA_HOME/lib
  ./a.out

  # verify that the new test file was created
  $HADOOP_HOME/bin/hdfs dfs  -ls /tmp


HDFS Support Types and Functions
--------------------------------

 

.. function:: proc connect(nameNode: string = "default", port: int = 0) throws

   
   
   Connect to an HDFS filesystem. If ``nameNode`` or ``port`` are not provided,
   the HDFS defaults will be used.
   
   :arg nameNode: the hostname for an HDFS name node to connect to
   :arg port: the port on which the HDFS service is running on the name node
   :returns: a :record:`hdfs` representing the connected filesystem.
   

.. record:: hdfs

   
   
   Record storing an open HDFS filesystem. Please see :class:`HDFSFileSystem` for
   the forwarded methods available, in particular :proc:`HDFSFileSystem.open`.
   
   


.. class:: HDFSFileSystem

   
   Class representing a connected HDFS file system. This connected is
   reference counted and shared by open files.
   


   .. method:: proc open(path: string, mode: iomode, style: iostyle = defaultIOStyle(), in flags: c_int = 0, bufferSize: c_int = 0, replication: c_short = 0, blockSize: tSize = 0) throws

      
      
      Open an HDFS file stored at a particular path.  Note that once the file is
      open, you will need to use :proc:`IO.file.reader` or :proc:`IO.file.writer`
      to create a channel to actually perform I/O operations.
      
      :arg path: which file to open (for example, "some/file.txt").
      :arg iomode: specify whether to open the file for reading or writing and whether or not to create the file if it doesn't exist.  See :type:`IO.iomode`.
      :arg style: optional argument to specify I/O style associated with this file.  The provided style will be the default for any channels created for on this file, and that in turn will be the default for all I/O operations performed with those channels.
      :arg flags: flags to pass to the HDFS open call. Uses flags appropriate for ``mode`` if not provided.
      :arg bufferSize: buffer size to pass to the HDFS open call.  Uses the HDFS default value if not provided.
      :arg replication: replication factor to pass to the HDFS open call.  Uses the HDFS default value if not provided.
      :arg blockSize: blockSize to pass to the HDFS open call.  Uses the HDFS default value if not provided.
      

