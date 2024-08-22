use CTypes, IO;

// The purpose of this test is to check that the allocations
// made in readBytesOrString have reasonable size and that there
// is not just a gigantic allocation for all cases.
//
// However, it is a bit inexact, because the allocator routinely
// rounds up allocations to a next larger size. It is possible that
// the assertions will need adjustment if the allocator changes.
// In that event, run it with --verbose=true and check that the
// allocation sizes are reasonable.
config const verbose = false;

proc getAllocationSize(s) {
  // use Chapel internals to compute the allocation size for
  // the string's buffer
  extern proc chpl_real_alloc_size(arg: c_ptr(void)): c_size_t;

  var size = chpl_real_alloc_size(s.buff):int;

  if size < s.buffSize {
    // handle the case where chpl_real_alloc_size doesn't do anything
    size = s.buffSize;
  }

  return size;
}

// for these cases, the readBytesOrString uses an initial allocation size
// based upon the file size
proc testBytesLargeFile(maxSize: int) {
  if verbose then
    writeln("testBytesLargeFile ", maxSize);

  var f = openReader("./moby.txt", locking=false);
  const s = f.readBytes(maxSize);
  const sz = getAllocationSize(s);

  if verbose then 
    writeln("testBytesLargeFile read ", maxSize, " and got allocation ", sz);

  assert(sz < 1.5 * maxSize + 64);
}

proc testStringLargeFile(maxSize: int) {
  if verbose then
    writeln("testStringLargeFile ", maxSize);

  var f = openReader("./moby.txt", locking=false);
  const s = f.readString(maxSize);
  const sz = getAllocationSize(s);

  if verbose then
    writeln("testStringLargeFile read ", maxSize, " and got allocation ", sz);

  assert(sz < 1.5 * maxSize + 64);
}

var a = "a";
var bigstr = a*20000;

// for memory file cases, readBytesOrString doesn't currently
// understand the file size
proc testBytesSmallMemFile(maxSize: int) {
  if verbose then
    writeln("testBytesSmallMemFile ", maxSize);

  var m = openMemFile();
  m.writer(locking=false).write(bigstr);

  var f = m.reader(locking=true);

  const s = f.readBytes(maxSize);
  const sz = getAllocationSize(s);

  if verbose then
    writeln("testBytesSmallMemFile read ", maxSize, " and got allocation ", sz);

  assert(sz < 2*maxSize + 64);
}

proc testStringSmallMemFile(maxSize: int) {
  if verbose then
    writeln("testStringSmallMemFile ", maxSize);

  var m = openMemFile();
  m.writer(locking=false).write(bigstr);

  var f = m.reader(locking=true);

  const s = f.readString(maxSize);
  const sz = getAllocationSize(s);

  if verbose then
    writeln("testStringSmallMemFile read ", maxSize, " and got allocation ", sz);

  assert(sz < 2*maxSize + 64);
}

var sizes = [50, 100, 500, 1000, 5000, 10000];
for s in sizes {
  testBytesLargeFile(s);
}
for s in sizes {
  testStringLargeFile(s);
}
for s in sizes {
  testBytesSmallMemFile(s);
}
for s in sizes {
  testStringSmallMemFile(s);
}

writeln("done");
