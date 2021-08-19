use FileSystem;
use IO;

proc main() throws {
  for f in myfindfiles() {
    var filereader = open(f, iomode.r).reader();
  }
}

/*
 * Copies of findfiles() and walkdirs() from FileSystem so that this
 * test covers this particular recursive iterator setup, no matter
 * what happens to the iterators in the standard module.
 *
 * "use FileSystem" above to call listdir().
 */
iter myfindfiles(startdir: string = ".", recursive: bool = false,
               hidden: bool = false): string {
  if (recursive) then
    foreach subdir in mywalkdirs(startdir, hidden=hidden) do
      foreach file in listdir(subdir, hidden=hidden, dirs=false, files=true, listlinks=true) do
        yield subdir+"/"+file;
  else
    foreach file in listdir(startdir, hidden=hidden, dirs=false, files=true, listlinks=false) do
      yield startdir+"/"+file;
}


iter mywalkdirs(path: string = ".", topdown: bool = true, depth: int = max(int),
              hidden: bool = false, followlinks: bool = false,
              sort: bool = false): string {

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir(path, hidden=hidden, files=false, listlinks=followlinks);
    if (sort) {
      use Sort /* only sort */;
      sort(subdirs);
    }

    for subdir in subdirs {
      const fullpath = path + "/" + subdir;
      for subdir in mywalkdirs(fullpath, topdown, depth-1, hidden,
                             followlinks, sort) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}
