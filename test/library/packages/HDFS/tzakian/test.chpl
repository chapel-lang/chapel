use HDFS, HDFStools, IO;
use OS.POSIX;

bar("default");

proc bar(nm: string) {

  var dataFile = "/tmp/test.txt";

  const hdfsFS: c_ptr(void) = HDFS.hdfsConnect(c_ptrToConst_helper(nm.localize()), 0);
  const fileInfo = HDFS.chadoopGetFileInfo(hdfsFS, c_ptrToConst_helper(dataFile.localize()));
  const blockHosts = HDFS.hdfsGetHosts(hdfsFS, c_ptrToConst_helper(dataFile.localize()), 0, fileInfo.mSize); // incr 0?
  const blockCount = HDFS.chadoopGetBlockCount(blockHosts);
  const dataFileLocal = HDFS.hdfsOpenFile(hdfsFS, c_ptrToConst_helper(dataFile.localize()), O_RDONLY, 0, 0, 0);
  assert(HDFS.IS_NULL(dataFileLocal) == HDFS.IS_NULL_FALSE, "Failed to open dataFileLocal");
  var length = fileInfo.mBlockSize: int(32); // LOOK

  writeln("size of file is: ", fileInfo.mSize);
  var s = HDFS.chadoopReadFile(hdfsFS, dataFileLocal, fileInfo.mSize: int(32)):string;

  var outfile = open("foo.txt", ioMode.cw);
  var writer = outfile.writer();

  writer.write(s);

  writer.close();
  outfile.close();
}
