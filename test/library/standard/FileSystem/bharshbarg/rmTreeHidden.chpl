
use FileSystem, IO;

const dirName = "rmTree_test";

proc makeHiddenFile(dir:string, fname:string) {
  var fi = open(dir + "/." + fname, iomode.cw);
  var w  = fi.writer();
  w.writeln("Hello, world!");
  w.close();
  fi.close();
}

proc main() {
  assert(isDir(dirName) == false);

  mkdir(dirName);

  assert(isDir(dirName) == true);

  makeHiddenFile(dirName, "hidden");
  makeHiddenFile(dirName, "foo");

  const nested = dirName + "/.nested";
  mkdir(nested);
  makeHiddenFile(nested, "foobar");

  rmTree(dirName);

  assert(isDir(dirName) == false);
}
