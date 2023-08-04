// TODO
//extern type tSize = int(32);

// Record returned by HDFS
extern record chadoopFileInfo {
  var mLastMod: int(32);
  var mSize: int(64);
  var mReplication: int(16);
  var mBlockSize: int(64);
  var mPermissions: int(16);
  var mLastAccess: int(32);
}

/* chadoop.h */
extern const IS_NULL_FALSE: c_int; // To see if it is null from 
extern const IS_NULL_TRUE: c_int;

extern proc printBlockHosts_C(b: c_ptr(void), l: c_string);

extern proc IS_NULL(ptr: c_ptr(void)): c_int;

extern proc chadoopFree(ptr: c_ptr(void));
extern proc chadoopFreeString(str: c_string);

extern proc chadoopGetFileInfo(hdfsFS: c_ptr(void), path: c_string): chadoopFileInfo;

extern proc chadoopGetBlockCount(hostBlocks: c_ptr(void)): int(32);
extern proc chadoopGetHostCount(hostBlocks: c_ptr(void), block: int(32)): int(32);
extern proc chadoopGetHost(hostBlocks: c_ptr(void), block: int(32), host: int(32)): c_string;

extern proc chadoopReadFile(hdfsFS: c_ptr(void), hdfsFile: c_ptr(void), length: int(32)): c_string;
extern proc chadoopReadFilePositional(hdfsFS: c_ptr(void), hdfsFile: c_ptr(void), position: int(64), length: int(32)): c_string;

/* libhdfs */

extern proc hdfsCloseFile(hdfsFS: c_ptr(void), hdfsFile: c_ptr(void)): c_int;

/* Returns hdfsFS */
extern proc hdfsConnect(nn: c_string, port: c_int): c_ptr(void);

/* Returns tOffset (int64) */
extern proc hdfsGetDefaultBlockSize(hdfsFS: c_ptr(void)): int(64);

extern proc hdfsFlush(hdfsFS: c_ptr(void), hdfsFile: c_ptr(void)): c_int;

/* Returns char*** -- see chadoopGetHosts/Blocks for more info */
extern proc hdfsGetHosts(hdfsFS: c_ptr(void), path: c_string, start: int(64), length: int(64)): c_ptr(void);

/* Returns hdfsFile */
extern proc hdfsOpenFile(hdfsFS: c_ptr(void), path: c_string, flags: c_int, bufferSize: c_int, replication: int(16), blocksize: int(32)): c_ptr(void);

/* Returns tSize (int32) */
extern proc hdfsWrite(hdfsFS: c_ptr(void), hdfsFile: c_ptr(void), buffer: c_string, length: int(32)): int(32);

