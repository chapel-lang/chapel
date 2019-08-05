use HDFS, Sys, HDFStools, IO;

bar("default");

proc bar(nm: string) {

  var dataFile = "/tmp/test.txt";

  const hdfsFS: c_void_ptr = HDFS.hdfsConnect(nm.localize().c_str(), 0);
  const fileInfo = HDFS.chadoopGetFileInfo(hdfsFS, dataFile.localize().c_str());
  const blockHosts = HDFS.hdfsGetHosts(hdfsFS, dataFile.localize().c_str(), 0, fileInfo.mSize); // incr 0?
  const blockCount = HDFS.chadoopGetBlockCount(blockHosts);
  const dataFileLocal = HDFS.hdfsOpenFile(hdfsFS, dataFile.localize().c_str(), O_RDONLY, 0, 0, 0);
  assert(HDFS.IS_NULL(dataFileLocal) == HDFS.IS_NULL_FALSE, "Failed to open dataFileLocal");
  var length = fileInfo.mBlockSize: int(32); // LOOK

  writeln("size of file is: ", fileInfo.mSize);
  var s = HDFS.chadoopReadFile(hdfsFS, dataFileLocal, fileInfo.mSize: int(32)):string;

  var outfile = open("foo.txt", iomode.cw);
  var writer = outfile.writer();

  writer.write(s);

  writer.close();
  outfile.close();
}


